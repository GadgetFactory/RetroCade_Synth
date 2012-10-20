#ifndef __RAMFS_H__
#define __RAMFS_H__

#ifdef __linux__

#else
#include "zpuino.h"
#endif

#include <SD.h>

/** RAMFS filesystem magic */
#define RAMFS_MAGIC 0x50411F50

#ifndef SEEK_SET
# define SEEK_SET   0
# define SEEK_CUR  1
# define SEEK_END  2
#endif

struct ramfs_header {
	unsigned int magic /** big-endian, magic number **/;
	unsigned int numfiles;
}__attribute__((packed));

struct ramfs_entry {
	unsigned int offset;
	unsigned int size;
	unsigned char namesize;
	char name[0];
} __attribute__((packed));

extern "C" void *bootloaderdata;
struct boot_t {
	unsigned int spiend;
};
/**
 * @brief RAMFS File Class
 */
class RamFSFile
{
public:
	RamFSFile(File *file);
	//RamFSFile(unsigned o,unsigned size): flashoffset(o),filesize(size),seekpos(0) {}
	/**
	 * @brief Check if file was successfuly opened.
	 * @return true on success, false otherwise
     */
	//bool valid() { return flashoffset>=0;}//!=-1; }

	/**
	 * @brief Read a chunk of data from file.
	 * @param buf The buffer where to store data
	 * @param size The number of bytes to read from file.
	 * @return The number of bytes read, 0 for EOF.
	 */
	int read(void *buf, int size);
	/**
	 * @brief Seek current file position
	 * @param pos The required position
	 * @param whence Where to perform seek. Either SEEK_SET, SEEK_CUR or SEEK_END
	 */
	void seek(int pos, int whence);
	/**
	 * @brief Get the file size.
	 * @return The file size.
     */
	inline int size() const { return filesize; }
	/**
	 * @brief Read a chunk of data from file, using a callback function.
	 * The function will be called for every byte read.
	 * @param size The number of bytes to read from file.
	 * @param callback The callback function to call
	 * @param data The data parameter to pass to callback function.
	 * @return The number of bytes read, 0 for EOF.
	 */
	
	//int readCallback(int size, void (*callback)(unsigned char, void*), void *data);

	/**
	 * @brief Read a single byte
	 */
	unsigned readByte();

private:
	int flashoffset;
	int filesize;
	int seekpos;
        char *data;
};

/**
 * @brief Main filesystem class
 */
class RamFS_class {
    friend class RamFSFile;
public:
	/**
	 * @brief Initialize the RamFS filesystem
	 * @return 0 on success, -1 otherwise
	 */
	int begin();

protected:
//	void spi_disable()
//	{
//#ifndef __linux__
//		digitalWrite(SPI_FLASH_SEL_PIN,HIGH);
//#endif
//	}
//
//	static void spi_enable()
//	{
//#ifndef __linux__
//		digitalWrite(SPI_FLASH_SEL_PIN,LOW);
//#endif
//	}
//
//	static inline void spiwrite(unsigned int i)
//	{
//#ifndef __linux__
//		SPIDATA=i;
//#endif
//	}
//
//	static inline unsigned int spiread()
//	{
//#ifndef __linux__
//		return SPIDATA;
//#else
//		return 0;
//#endif
//	}
//
//	static inline void spiwrite(register_t datareg, unsigned int i)
//	{
//#ifndef __linux__
//		*datareg=i;
//#endif
//	}
//
//	static inline unsigned int spiread(register_t datareg)
//	{
//#ifndef __linux__
//		return *datareg;
//#else
//		return 0;
//#endif
//	}

protected:
	void read(unsigned address, void *target, unsigned size);
	void seek(unsigned address) { seek_if_needed(address); }
	unsigned readByte(unsigned address);
public:
	/**
	 * @brief Open a file on the filesystem.
	 * @param name The file name
	 * @return A new RamFSFile object. You should call valid() to check
	 * if file was successfully open.
	 */
	RamFSFile open(File *file);

private:
	void seek_if_needed(unsigned address);

	struct ramfs_header hdr;
	unsigned fsstart;
	unsigned offset;
#ifdef __linux__
	int fd;
#endif
};

extern RamFS_class RamFS;

#endif

