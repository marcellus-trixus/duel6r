/*
* Copyright (c) 2006, Ondrej Danek (www.ondrej-danek.net)
* All rights reserved.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Ondrej Danek nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Math.h"
#include "WaterList.h"

namespace Duel6
{
	WaterList::WaterList()
		: phase(0.0f)
	{}

	void WaterList::build(FaceList& waterFaces, Float32 waveHeight)
	{
		this->waveHeight = waveHeight;
		vertexes.clear();
		for (Vertex& vertex : waterFaces.getVertexes())
		{
			if (vertex.getFlag() == Vertex::Flag::Flow)
			{
				vertexes.push_back(WaterVertex(vertex, waveHeight));
			}
		}
	}

	void WaterList::update(Float32 elapsedTime)
	{
		phase += 122 * elapsedTime;
		while (phase >= 360)
		{
			phase -= 360;
		}

		for (WaterVertex& wv : vertexes)
		{
			Vertex& vertex = wv.getVertex();
			Float32 vertexPhase = phase + 60.0f * vertex.x;
			Float32 height = Math::fastSin((Int32)vertexPhase) * waveHeight;
			vertex.y = wv.getY() + height;
		}
	}
}