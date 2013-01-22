/*!
 *  @file		ramFS.cpp
 *  Project		RamFS Library
 *	@brief		Allows SD or SmallFS files to be loaded into SDRAM.
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		11/10/12
 *  License		GPL
 */

#include "ramFS.h"
#include <string.h>
#ifdef __linux__

#include <fcntl.h>
#include <unistd.h>
#include <endian.h>
#include <stdio.h>
#include <new.h>

#define BE32(x) be32toh(x)

#else

#include "WProgram.h"

#define BE32(x) x

#endif

#undef RAMFSDEBUG

File modSDfile;

extern void*__end__;
unsigned long *mbuf = (unsigned long*)&__end__+80000;

RamFSFile::RamFSFile(){
  
}

void *RamFSFile::zpuinomalloc(unsigned long size)
{
 void *ret = mbuf;
 mbuf+=(size);
 return ret;
}

void RamFSFile::setInit(boolean active)
{
//  initState = active;
}
void RamFSFile::init()
{
//  Serial.println("Running malloc.");
//  moddata = (unsigned char *)zpuinomalloc(500000*sizeof(unsigned char));
//  memset(moddata, 0, 500000*sizeof(unsigned char));    
//  initState = true;
}

int RamFS_class::begin()
{
  return 0;
}

void RamFS_class::seek_if_needed(unsigned long address)
{

}

unsigned RamFS_class::readByte(unsigned long address)
{
    return 0;

}


void RamFS_class::read(unsigned long address, void *target, unsigned long size)
{

}

RamFSFile RamFS_class::open(SmallFSFile *file)
{
        //Serial.println("In RamFS.open");
	return RamFSFile(file);
}

RamFSFile::RamFSFile(SmallFSFile *file)
{
        filesize = file->size();
        file->seek(0x0, SEEK_SET);     
        moddata = (unsigned char *)malloc(filesize*sizeof(unsigned char));
        memset(moddata, 0, filesize*sizeof(unsigned char));
        file->read(&moddata[0], filesize);   
}

RamFSFile RamFS_class::open(File *file)
{
	return RamFSFile(file);
}

RamFSFile::RamFSFile(File *file)
{
        unsigned long i = 0;
        filesize = file->size();  
        file->seek(0x0);         
        moddata = (unsigned char *)malloc(filesize*sizeof(unsigned char));
        memset(moddata, 0, filesize*sizeof(unsigned char));
        //file->read(&moddata[0], filesize);   //TODO: Use this more efficient form when the SD library is fixed.      
        while (file->available()) {
          moddata[i] = file->read();
          i++;
        }        
}

int RamFSFile::read(void *buf, unsigned long s)
{ 

	if (seekpos==filesize)
		return 0; /* EOF */

	if (s + seekpos > filesize) {
		s = filesize-seekpos;
	}
        memcpy(buf, moddata+seekpos, s);

	seekpos+=s;
	return s;  
}


void RamFSFile::seek(unsigned long pos, int whence)
{
	unsigned long newpos;

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
}

RamFS_class RamFS;

