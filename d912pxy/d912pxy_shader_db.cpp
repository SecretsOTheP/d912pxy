/*
MIT License

Copyright(c) 2018-2020 megai2

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
#include "stdafx.h"

d912pxy_shader_db::d912pxy_shader_db() 
{

}


d912pxy_shader_db::~d912pxy_shader_db()
{
}

void d912pxy_shader_db::Init()
{
	NonCom_Init(L"shader database");

	shaderPairs = new d912pxy_memtree2(sizeof(d912pxy_shader_pair_hash_type), 0xFF, 2);
}

void d912pxy_shader_db::UnInit()
{
	shaderPairs->Begin();

	while (!shaderPairs->IterEnd())
	{
		UINT64 cid = shaderPairs->CurrentCID();
		if (cid)
		{
			d912pxy_shader_pair* item = (d912pxy_shader_pair*)cid;

			delete item;
		}
		shaderPairs->Next();
	}

	delete shaderPairs;

	d912pxy_noncom::UnInit();
}

d912pxy_shader_uid d912pxy_shader_db::GetUID(DWORD * code, UINT32* len)
{
	UINT hash = 0x811c9dc5;
	UINT ctr = 0;

	while (code[ctr >> 2] != 0x0000FFFF)
	{
		UINT8 dataByte = ((UINT8*)code)[ctr];

		hash = hash ^ dataByte;
		hash = hash * 0x01000193;
		++ctr;
	}

	*len = (ctr >> 2) + 1;

	return hash;
}

d912pxy_shader_pair_hash_type d912pxy_shader_db::GetPairUID(d912pxy_shader * vs, d912pxy_shader * ps)
{
	d912pxy_shader_uid pdc[2] = { vs->GetID(), ps->GetID() };

	LOG_DBG_DTDM2("ShaderPair %016llX %016llX", pdc[0], pdc[1]);

	d912pxy_shader_pair_hash_type ha = (d912pxy_shader_pair_hash_type)(pdc[0] ^ pdc[1]);

	return ha;
}

d912pxy_shader_pair * d912pxy_shader_db::GetPair(d912pxy_shader* vs, d912pxy_shader* ps)
{	
	d912pxy_shader_pair_hash_type ha = GetPairUID(vs, ps);
		
	d912pxy_shader_pair* it = (d912pxy_shader_pair*)shaderPairs->PointAtMemMTR(&ha, sizeof(d912pxy_shader_pair_hash_type));
	
	if (it)
	{		
		return it;
	}
	else {
		it = (d912pxy_shader_pair*)shaderPairs->PointAtMemMTRW(&ha, sizeof(d912pxy_shader_pair_hash_type));

		if (!it)
		{
			d912pxy_shader_uid pdc[2] = { vs->GetID(), ps->GetID() };

			it = new d912pxy_shader_pair(ha, pdc);

			vs->NotePairUsage(ha);
			ps->NotePairUsage(ha);
		}
	
		shaderPairs->PointAtMemMTW((intptr_t)it);

		return it;
	}
}

void d912pxy_shader_db::DeletePair(d912pxy_shader_pair_hash_type ha)
{
	d912pxy_shader_pair* it = (d912pxy_shader_pair*)shaderPairs->PointAtMemMTRW(&ha, sizeof(d912pxy_shader_pair_hash_type));

	shaderPairs->PointAtMemMTW(0);
	
	if (it)
		delete it;		
}
