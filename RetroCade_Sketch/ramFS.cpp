#include "RamFS.h"
#include <string.h>
#ifdef __linux__

#include <fcntl.h>
#include <unistd.h>
#include <endian.h>
#include <stdio.h>

#define BE32(x) be32toh(x)

#else

#include "WProgram.h"

#define BE32(x) x

#endif

#undef RAMFSDEBUG

int RamFS_class::begin()
{

//#ifdef __linux__
//	fsstart = 0;
//	fd = ::open("ramfs.dat",O_RDONLY);
//	if (fd<0) {
//		perror("Cannot open ramfs.dat");
//		return -1;
//	}
//#else
//	struct boot_t *b = (struct boot_t*)bootloaderdata;
//	fsstart = b->spiend;
//
//#endif
//	offset = 0xffffffff;
//
//	read(fsstart, &hdr,sizeof(hdr));
//
//#ifdef RAMFSDEBUG
//
//	unsigned debug;
//
//	Serial.print("Read magic ");
//	Serial.println(hdr.magic);
//	Serial.print("SPI end ");
//	Serial.println(fsstart);
//
//	Serial.print("Bdata at ");
//	Serial.println((unsigned)b);
//	// Test read
//
//	read(fsstart - 8, &debug,sizeof(debug));
//	Serial.print("DD1 ");
//	Serial.println(debug);
//	read(&debug,sizeof(debug));
//	Serial.print("DD2 ");
//	Serial.println(debug);
//	read(&debug,sizeof(debug));
//	Serial.print("DD3 ");
//	Serial.println(debug);
//
//#endif
//
//	if(BE32(hdr.magic) == RAMFS_MAGIC)
//		return 0;
//	return -1;
}

void RamFS_class::seek_if_needed(unsigned address)
{
//	register_t spibasedata=&SPIDATA;
//
//	if (address!=offset)
//	{
//		offset = address;
//		spi_disable();
//		spi_enable();
//#ifdef RAMFSDEBUG
//		Serial.print("Seeking to ");
//		Serial.println(address);
//#endif
//		/*
//		spiwrite(spibasedata,0x0B);
//		spiwrite(spibasedata,address>>16);
//		spiwrite(spibasedata,address>>8);
//		spiwrite(spibasedata,address);
//		spiwrite(spibasedata,0);
//		spiwrite(spibasedata,0); // Read ahead
//		*/
//		address+=0x0B000000;
//		spiwrite(spibasedata+2,address>>16); /* 16-bit write */
//		address<<=16;
//		spiwrite(spibasedata+6,address); /* 32-bit Includes read-ahead */
//	}
}

unsigned RamFS_class::readByte(unsigned address)
{
//	seek_if_needed(address);
//	unsigned v = spiread(); // Already cached
//	spiwrite(0);
//	offset++;
//	return v;
}


void RamFS_class::read(unsigned address, void *target, unsigned size)
{
//#ifdef __linux__
//	if (fd>=0) {
//		::read(fd,target,size);
//	}
//#else
//	seek_if_needed(address);
//
//	unsigned char *p=(unsigned char*)target;
//	while (size--) {
//		unsigned v = spiread(); // Already cached
//		spiwrite(0);
//		*p++=v;
//		offset++;
//	}
//#endif
}

RamFSFile RamFS_class::open(File *file)
{
//	/* Start at root offset */
//	unsigned o = fsstart + sizeof(struct ramfs_header);
//	unsigned char buf[256];
//	struct ramfs_entry e;
//
//	int c;
//
//	for (c=BE32(hdr.numfiles); c; c--) {
//
//		read(o, &e,sizeof(struct ramfs_entry));
//		o+=sizeof(struct ramfs_entry);
//
//		read(o, buf,e.namesize);
//		o+=e.namesize;
//
//		buf[e.namesize] = '\0';
//		/* Compare */
//		if (strcmp((const char*)buf,name)==0) {
//			
//			// Seek and readahead
//			seek_if_needed(BE32(e.offset) + fsstart);
//
//			return RamFSFile(BE32(e.offset) + fsstart, BE32(e.size));
//		}
//	}
//	// Reset offset.
//	offset=(unsigned)-1;

	return RamFSFile(file);
}

RamFSFile::RamFSFile(File *file)
{
        //int len = 0;
        //pt_mod_s *mod;
        //char *buf=0;
        
        filesize = file->size();
        
        data = (char *)malloc(filesize * sizeof(char));
        file->read(data,filesize);  
}

int RamFSFile::read(void *buf, int s)
{
//	if (!valid())
//		return -1;

	if (seekpos==filesize)
		return 0; /* EOF */

	if (s + seekpos > filesize) {
		s = filesize-seekpos;
	}
	//RamFS.read(seekpos + flashoffset, buf,s);
        buf = data + seekpos;

	seekpos+=s;
	return s;  
}

//int RamFSFile::readCallback(int s, void (*callback)(unsigned char, void*), void *data)
//{
//	unsigned char c;
//	int save_s;
//
//	if (!valid())
//		return -1;
//
//	if (seekpos==filesize)
//		return 0; /* EOF */
//
//	if (s + seekpos > filesize) {
//		s = filesize-seekpos;
//	}
//#ifdef RAMFSDEBUG
//	Serial.print("Will read ");
//	Serial.print(s);
//	Serial.print(" bytes from file at offset ");
//	Serial.print(flashoffset);
//	Serial.print(" seekpos is ");
//
//	Serial.println(seekpos);
//#endif
//
//	//RamFS.spi_enable();
//
//	//RamFS.startread( seekpos + flashoffset );
//	save_s = s;
//	unsigned tpos = seekpos + flashoffset;
//	seekpos += s;
//
//	while (s--) {
//		c=RamFS.readByte(tpos++);
//		callback(c,data);
//	}
//
//	return save_s;
//}

void RamFSFile::seek(int pos, int whence)
{
	int newpos;

	if (whence==SEEK_SET)
		newpos = pos;
	else if (whence==SEEK_CUR)
		newpos = seekpos + pos;
	else
		newpos = filesize + pos;

	if (newpos>filesize)
		newpos=filesize;

	if (newpos<0)
		newpos=0;

	seekpos=newpos;
	//RamFS.seek(seekpos + flashoffset);
}

RamFS_class RamFS;

