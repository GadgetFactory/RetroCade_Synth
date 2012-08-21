#include <SD.h>
#include <endian.h>
#include "cbuffer.h"

#if defined( __ZPUINO_PAPILIO_ONE__) || defined (__ZPUINO_PAPILIO_PLUS__)

Sd2Card card;

// Connections to SD card
//#define CSPIN  WING_C_4
//#define SDIPIN WING_C_3
//#define SCKPIN WING_C_2
//#define SDOPIN WING_C_1

//Arcade MegaWing
#define CSPIN  WING_C_12
#define SDIPIN WING_C_13
#define SCKPIN WING_C_14
#define SDOPIN WING_C_15

// Audio output
#define SIGMADELTA0PIN WING_C_8
#define SIGMADELTA1PIN WING_C_10

#define FPGA_LED_0 WING_A_0
#define FPGA_LED_1 WING_A_1
#define FPGA_LED_2 WING_A_2
#define FPGA_LED_3 WING_A_3
#define FPGA_LED_4 WING_A_4
#define FPGA_LED_5 WING_A_5
#define FPGA_LED_6 WING_A_6
#define FPGA_LED_7 WING_A_7

#endif

#ifdef __ZPUINO_S3E_EVAL__

#define SDOPIN FPGA_PIN_A6
#define SCKPIN FPGA_PIN_B6
#define CSPIN  FPGA_PIN_E7
#define SDIPIN FPGA_PIN_F7

#define SIGMADELTA0PIN FPGA_PIN_C7
#define SIGMADELTA1PIN FPGA_PIN_F8

#endif

static CircularBuffer<unsigned int,9> buffer; // 7 bits (128 samples), 512 byte

#define RIFF_ID 0x52494646
#define WAVE_ID 0x57415645

struct riff_header
{
	__be32 chunkid;
	__le32 chunksize;
	__be32 format;
};

struct wav_fmt
{
	__be32 chunkid;
	__le32 chunksize;
	__le16 audioformat;
	__le16 numchannels;
	__le32 samplerate;
	__le32 byterate;
	__le16 blockalign;
	__le16 bps;
};

struct data_fmt
{
	__le32 chunkid;
	__le32 size;
};

unsigned int run=0;
unsigned int underruns=0;
unsigned int pos=0;

void _zpu_interrupt()
{
	if (run) {
		if (buffer.hasData()) {
			// Buffer has data
			SIGMADELTADATA = buffer.pop();
			pos++;
		} else {
			SIGMADELTADATA=0x80008000;
			underruns++;
		}
	} else {
		SIGMADELTADATA=0x80008000;
	}
	TMR0CTL &= ~_BV(TCTLIF);
}

void start()
{
	reset();
	underruns=0;
	run = 1;
}

void stop()
{
	run = 0;
}

void reset()
{
	stop();
	buffer.clear();
}

void set_sample_rate(unsigned int samplerate)
{
	// Stop timer first
	TMR0CTL = 0;
	TMR0CNT = 0;
	TMR0CMP = ((CLK_FREQ/2) / samplerate )- 1;
	TMR0CTL = _BV(TCTLENA)|_BV(TCTLCCM)|_BV(TCTLDIR)|
		_BV(TCTLCP0) | _BV(TCTLIEN);
}

unsigned int lastled;

void setled(unsigned v)
{
	unsigned led = ledforvalue(v);
	digitalWrite(lastled,LOW);
	lastled = led;
	digitalWrite(led,HIGH);
}

static const unsigned int ledpos[] = { FPGA_LED_0, FPGA_LED_1, FPGA_LED_2, FPGA_LED_3, FPGA_LED_4, FPGA_LED_5 ,FPGA_LED_6, FPGA_LED_7 };

unsigned ledforvalue(unsigned value)
{
	return ledpos[value&0x7];
}

void setledupto(int value) {
	unsigned i;
	value&=0x7;
	for (i=0;i<=7;i++) {
		digitalWrite(ledpos[i],LOW);
	}

	do {
		digitalWrite(ledpos[value--],HIGH);
	} while (value>=0);
}

File myFile;

uint8_t tmpbuf[512];

extern void dumpbuffer(unsigned char*);

int readdata(unsigned int size)
{
	unsigned bytes=0;
	Serial.print("Playing ");
	Serial.print(size,10);

	Serial.println(" bytes.");

	unsigned max = 0;
	unsigned count = 0;

	while (size) {
		int retries=10;
		unsigned chunksize = size >= 512?512:size;
		do_retry:
		int r = myFile.read(tmpbuf,chunksize);
		if (r<=0) {
			Serial.print("Read error at minus ");
			Serial.println(size,10);
			Serial.print("Error code: ");
			Serial.print(SD.errorCode(),10);
			Serial.print(" data ");
			Serial.println(SD.errorData(),10);

			if (retries--)
				goto do_retry;

			break;
		}

		unsigned int *src = (unsigned int*)tmpbuf;

		if(!run)
			start();

		for (int i=0;i<128;i++) {
			while (buffer.isFull()) {
			}
			unsigned v = *src++ ^ 0x00800080;
			buffer.push(v);
			// Fetch upper 8 bits from a sample.
			v&=0xff;
			v-=127;
			v = abs((int)v);
			max+=v;
			count++;
		}

		if (count==1024) {
			// Update LEDs
			max/=1024;
			max>>=2;
			setledupto(max);
			count=0;
			max=0;
		}

		//max/=128;

		// Update leds

		//max = max >> 3;

		//setled(max);

		bytes+=r;
		size-=r;

		if ( (bytes & 0xffff) == 00) {
			Serial.print(bytes);
			Serial.print(" ");
            Serial.print(underruns);
            Serial.print("      \r");
		}
	};
	Serial.print("\n");
	Serial.print("Underruns: ");
	Serial.println(underruns,10);
}

int play()
{
	/* Read header */
	struct riff_header *hdr = (struct riff_header*)tmpbuf;
	struct wav_fmt *fmt = (struct wav_fmt*)tmpbuf;
	struct data_fmt *dfmt = (struct data_fmt*)tmpbuf;
	unsigned pos = 0;

	int r = myFile.read(tmpbuf,sizeof(struct riff_header));
	if (r<=0)
		return r;
	pos+=sizeof(struct riff_header);

	if (hdr->chunkid != RIFF_ID) {
		//Serial.println("Not a RIFF file");
		return -1;
	}
    if (hdr->format != WAVE_ID) {
	   // Serial.println("Not a WAV file");
		return -1;
	}

	r = myFile.read(tmpbuf,sizeof(struct wav_fmt));
	pos+=sizeof(struct wav_fmt);

    if (r<=0)
		return r;

	if (fmt->chunkid!=0x666d7420) {
	   // Serial.println("Invalid WAV file");
		return -1;
	}

	Serial.print("Info: ");
	Serial.print(fmt->numchannels,10);
	Serial.print(" channels, sample rate ");
	Serial.println(fmt->samplerate,10);

	reset();

	set_sample_rate(fmt->samplerate);

	// Read WAV file

	do {
		r = myFile.read(tmpbuf,sizeof(struct data_fmt));
		if (r<=0)
			return r;
		pos+=sizeof(struct data_fmt);

		if (fmt->chunkid!=0x64617461) {
		 //   Serial.println("Invalid chunk");
			return -1;
		}
		unsigned size = dfmt->size;

		/* Align to next block */
		myFile.read(tmpbuf,512-pos);

		Serial.println("Running now.");

		if(readdata(size)!=0) {
			Serial.println("Error reading.");
			return -1;
		}
	} while (1);
}



void setup()
{
	// Map pins
    /*
	pinMode(FPGA_LED_0,OUTPUT);
	pinMode(FPGA_LED_1,OUTPUT);
	pinMode(FPGA_LED_2,OUTPUT);
	pinMode(FPGA_LED_3,OUTPUT);
	pinMode(FPGA_LED_4,OUTPUT);
	pinMode(FPGA_LED_5,OUTPUT);
	pinMode(FPGA_LED_6,OUTPUT);
	pinMode(FPGA_LED_7,OUTPUT);
    */

	outputPinForFunction( SDIPIN, IOPIN_USPI_MOSI );
	pinModePPS(SDIPIN,HIGH);
	pinMode(SDIPIN,OUTPUT);

	outputPinForFunction( SCKPIN, IOPIN_USPI_SCK);
	pinModePPS(SCKPIN,HIGH);
	pinMode(SCKPIN,OUTPUT);

	pinModePPS(CSPIN,LOW);
	pinMode(CSPIN,OUTPUT);

	inputPinForFunction( SDOPIN, IOPIN_USPI_MISO );
	pinMode(SDOPIN,INPUT);

	INTRMASK = _BV(INTRLINE_TIMER0);
	INTRCTL = _BV(0); // Globally enable interrupts

	pinMode(SIGMADELTA0PIN,OUTPUT);
	pinModePPS(SIGMADELTA0PIN, HIGH);
	pinMode(SIGMADELTA1PIN,OUTPUT);
	pinModePPS(SIGMADELTA1PIN, HIGH);

	outputPinForFunction( SIGMADELTA0PIN, IOPIN_SIGMADELTA0);
	outputPinForFunction( SIGMADELTA1PIN, IOPIN_SIGMADELTA1);

	SIGMADELTACTL = _BV(SDENA0)|_BV(SDENA1)|_BV(2)|_BV(3);

	//USPICTL=BIT(SPICP1)|BIT(SPICPOL)|BIT(SPISRE)|BIT(SPIEN)|BIT(SPIBLOCK);
         USPICTL=BIT(SPICP1)|BIT(SPICP0)|BIT(SPICPOL)|BIT(SPISRE)|BIT(SPIEN)|BIT(SPIBLOCK);

	Serial.begin(115200);
}

void loop()
{
	int i;
	Serial.println("Starting");

	digitalWrite(CSPIN,LOW);

	for (i=0;i<51200;i++)
		USPIDATA=0xff;

	digitalWrite(CSPIN,HIGH);

	for (i=0;i<51200;i++)
		USPIDATA=0xff;

  if (!card.init(SPI_HALF_SPEED, CSPIN)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
   Serial.println("Wiring is correct and a card is present."); 
  }

	if (!SD.begin(CSPIN)) {
		Serial.println("init failed!");
		Serial.println(SD.errorCode());
	} else {
		Serial.println("done.");
		SD.ls();

//		myFile = SD.open("test.wav",O_RDONLY);
//		if (myFile) {
//			Serial.println("Opened");
//			play();
//		} else {
//			Serial.println("Cannot open");
//		}
	}
	while (!Serial.available());
	Serial.read();
}



