/*!
 *  @file		ramFS.h
 *  Project		RamFS Library
 *	@brief		Allows SD or SmallFS files to be loaded into SDRAM.
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		11/10/12
 *  License		GPL
 */

#ifndef __RAMFS_H__
#define __RAMFS_H__

#ifdef __linux__

#else
#include "zpuino.h"
#endif

#include <SD.h>
#include "SmallFS.h"

#ifndef SEEK_SET
# define SEEK_SET   0
# define SEEK_CUR  1
# define SEEK_END  2
#endif

/**
 * @brief RAMFS File Class
 */
class RamFSFile
{
public:
        RamFSFile();
	RamFSFile(File *file);
        RamFSFile(SmallFSFile *file);
        static void setInit(boolean active);
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
	int read(void *buf, unsigned long size);
	/**
	 * @brief Seek current file position
	 * @param pos The required position
	 * @param whence Where to perform seek. Either SEEK_SET, SEEK_CUR or SEEK_END
	 */
	void seek(unsigned long pos, int whence);
	/**
	 * @brief Get the file size.
	 * @return The file size.
     */
	inline unsigned long size() const { return filesize; }
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
        static void *zpuinomalloc(unsigned long size);

private:
        void init();
	int flashoffset;
	unsigned long filesize;
	unsigned long seekpos;
        unsigned char *moddata;
        static boolean initState;
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


protected:
	void read(unsigned long address, void *target, unsigned long size);
	void seek(unsigned long address) { seek_if_needed(address); }
	unsigned readByte(unsigned long address);
public:
	/**
	 * @brief Open a file on the filesystem.
	 * @param name The file name
	 * @return A new RamFSFile object. You should call valid() to check
	 * if file was successfully open.
	 */
	RamFSFile open(File *file);
        RamFSFile open(SmallFSFile *file);

private:
	void seek_if_needed(unsigned long address);

};

extern RamFS_class RamFS;

#endif

