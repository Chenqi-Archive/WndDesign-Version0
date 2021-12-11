#pragma once

#include "types.h"
#include "hheaTable.h"

struct longHorMetric {
	USHORT	advanceWidth;
	SHORT	lsb;
};

struct hmtxTable {
	longHorMetric* hMetrics;
	SHORT* leftSideBearing;

	USHORT numberOfHMetrics;
	USHORT numGlyphs;

	void read(const void* const data, uint offset, USHORT numberOfHMetrics, USHORT numGlyphs) {
		this->numberOfHMetrics = numberOfHMetrics;
		this->numGlyphs = numGlyphs;
		hMetrics = new longHorMetric[numberOfHMetrics];
		for (uint i = 0; i < numberOfHMetrics; ++i) { 
			ReadVar(hMetrics[i].advanceWidth); 
			ReadVar(hMetrics[i].lsb);
		}

		ReadVarArrayNew(SHORT, leftSideBearing, (uint)numGlyphs - numberOfHMetrics);
	}

	longHorMetric GetGlyphMetric(USHORT glyphID) const {
		if (hMetrics == nullptr || glyphID >= numGlyphs) { return {}; }

		if (glyphID < numberOfHMetrics) {
			return hMetrics[glyphID];
		}
		return { hMetrics[numberOfHMetrics - 1].advanceWidth,
				 leftSideBearing[glyphID - numberOfHMetrics] };
	}

	~hmtxTable() {
		delete[] hMetrics;
		delete[] leftSideBearing;
	}
};