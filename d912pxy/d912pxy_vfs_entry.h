/*
MIT License

Copyright(c) 2019 megai2

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#pragma once
#include "stdafx.h"

class d912pxy_vfs_entry : public d912pxy_noncom
{
public:
	d912pxy_vfs_entry(UINT id);
	~d912pxy_vfs_entry();

	UINT64 IsPresentH(UINT fnHash);
	void* GetFileDataH(UINT namehash, UINT* sz);

	void WriteFileH(UINT namehash, void* data, UINT sz);
	void ReWriteFileH(UINT namehash, void* data, UINT sz);

	void AddFileInfo(d912pxy_vfs_pck_chunk* fileInfo);
	void LoadFileFromDisk(d912pxy_vfs_pck_chunk* fileInfo);
	void LoadFilesFromDisk();

	d912pxy_memtree2* GetChunkTree() {
		return chunkTree;
	};

private:
	d912pxy_memtree2* chunkTree;
	
	UINT m_Id;
};