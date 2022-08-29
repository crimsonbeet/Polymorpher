/* Created by Alex Nikol Bourakov in 1989 */
/* Adopted for RtlsTether project on 23/11/2011 by Alex Bourakov */

#include "stdafx.h" 

#include <queue>

#include<MATH.H>
#include<STDIO.H>
#include "fqeval.hpp"
/**/
/**/
void ScalePoints(long s_factor)
{
	register long p;
	PtBead*Pt;
	IPOINT = IPOINT5;
	for(p = 1; p<ZPTLIM; ++p)
	{
		Pt = GetPtBead(p);
		Pt->ZPTCO1 <<= s_factor;
		Pt->ZPTCO2 <<= s_factor;
	}
}

void ScalePoints(_sAlong& x, _sAlong& y, long lim, long s_factor) {
	IPOINT = IPOINT5;
	for (long j = 0; j < lim; ++j) {
		x[j] <<= s_factor;
		y[j] <<= s_factor;
	}
}

void
Back_Scale_1(void)
{
	register long p, l;
	PtBead*Pt;

	const double sf = ((int64_t)1L)<<S_FACTOR;

	for(p = 1; p<ZPTLIM; ++p) {
		Pt = GetPtBead(p);
		//Pt->ZPTCO1 >>= S_FACTOR;
		//Pt->ZPTCO2 >>= S_FACTOR;
		Pt->ZPTCO1 = (long)floor((double)Pt->ZPTCO1/sf +0.5);
		Pt->ZPTCO2 = (long)floor((double)Pt->ZPTCO2/sf +0.5);
	}
	PXMIN >>= S_FACTOR;
	PYMIN >>= S_FACTOR;
	PXMAX >>= S_FACTOR;
	PYMAX >>= S_FACTOR;

	IPOINT = IPOINT4;
}

bool Back_Scale_2(void)
{
	register long X;
	register long Y;
	register long h;

	PtBead*Pt;
	PtBead*PT;

	bool pv_analysis_isNeeded = false;


	for(long p = 1; p<ZPTLIM; ++p)
	{
		Pt = GetPtBead(p);
		if (!Pt) {
			continue;
		}
		if(Pt->ZPTAux) {
			/* 
				Residual from previous iteration. 
				Skip it. 
			*/
			continue;
		}
		Y = Pt->ZPTCO2;
		X = Pt->ZPTCO1;

		h = GetPointHashNumber(X + Y);

		PT = ZPTHASH[h];
		while(PT)
		{
			if(PT->ZPTCO1==X&&PT->ZPTCO2==Y)break;
			PT = PT->ZPTNxt;
		}
		if(!PT)
		{
			Pt->ZPTNxt = ZPTHASH[h];
			ZPTHASH[h] = Pt;
			Pt->ZPTAux = 0;
		}
		else 
		{ 
			Pt->ZPTAux = PT;
			pv_analysis_isNeeded = true;
		} 
	}

	return pv_analysis_isNeeded; 
}

void Back_Scale_3(void)
{
	VtBead*Vr;
	VtBead*Vt;
	PtBead*Pt;
	PtBead*P1;
	PtBead*P2;
	register char FA, FB;

	long vt;
	for(vt = 2; vt<ZVTLIM; vt+=2)
	{
		FA = FB = 0;
		Vt = GetVtBead(vt);

		Vr = Vt->ZVTMeInreverse;
		if(!Vr)
		{
			continue;
		}
		P1 = Vt->ZVTCO1;
		P2 = Vt->ZVTCO2;

		Pt = (PtBead*)P1->ZPTAux;
		if(Pt)
		{
			Vt->ZVTCO1 = Vr->ZVTCO2 = Pt;
			FA = 1;
		}
		Pt = (PtBead*)P2->ZPTAux;
		if(Pt)
		{
			Vt->ZVTCO2 = Vr->ZVTCO1 = Pt;
			FB = 1;
		}
		if(FA||FB)
		{
			Relink_Lv_Pt(Vt);
			Relink_Lv_Pt(Vr);
		}
	}
}
void
Back_Scale_4(void)
{
	LbBead*Lb;
	PtBead*P1;
	PtBead*P2;
	LvBead*Lv;
	LvBead*Le;
	LvBead*Lo;
	VtBead*Vt;
	VtBead*Vr;
	long lb;
	for(lb = 1; lb<ZLBLIM; ++lb)
	{
		Lb = GetLbBead(lb);
		Lv = Lb->ZLBFirstLvBead;
		if(!Lv)
		{
			continue;
		}
		Le = Lv;
		do
		{
			P1 = Lv->ZLVPoint1;
			P2 = Lv->ZLVPoint2;
			if(P1==P2)Lv = Lv->ZLVNextLvBead;
		}
		while(Lv&&(P1==P2)&&(Lv!=Le));
		if(P1==P2)
		{
			Lv = Lb->ZLBFirstLvBead, Lb->ZLBFirstLvBead = 0;
			do
			{
				Vt = Lv->ZLVMyVector;
				if(Vt)
				{
					Vr = Vt->ZVTMeInreverse;
					Vt->ZVTMeInreverse = 0;
					if(Vr) {
						Vr->ZVTMeInreverse = 0;
					}
				}
				Lv = Lv->ZLVNextLvBead;
			}
			while(Lv&&(Lv!=Le));
			continue;
		}
		else if(Lv!=Le) Lb->ZLBFirstLvBead = Lv;
		Le = Lv;
		Lo = Le;
		Lv = Lv->ZLVNextLvBead;
		if(Lv)
		do
		{
			P1 = Lv->ZLVPoint1;
			P2 = Lv->ZLVPoint2;
			if(P1==P2)
			{
				Vt = Lv->ZLVMyVector;
				if(Vt)
				{
					Vr = Vt->ZVTMeInreverse;
					Vt->ZVTMeInreverse = 0;
					if(Vr)Vr->ZVTMeInreverse = 0;
				}
				Lo->ZLVNextLvBead = Lv->ZLVNextLvBead;
			}
			else Lo = Lv;
			Lv = Lv->ZLVNextLvBead; 
		}
		while(Lv&&(Le!=Lv));
	}
}


void CleanPTLinks()
{
	for (long p = 1; p < ZPTLIM; ++p) {
		auto pt = GetPtBead(p);
		pt->ZPTAux = 0;
	}
}

void CleanPvTable() {
	for (long p = 1; p < ZPTLIM; ++p) {
		auto pt = GetPtBead(p);
		pt->ZPTFirstSortedVtBead = 0;
	}
	for (long v = 2; v < ZVTLIM; ++v) {
		auto vt = GetVtBead(v);
		vt->ZVTNextSortedVtBead = 0;
	}
}



long Back_ScalePoints(long s_factor)
{
	int mergeIterations = 0;
	bool mergeIsDone = false;
	while(!mergeIsDone) {
		mergeIsDone = MergePoints(s_factor);
		++mergeIterations;
	}; 


	CleanZPTHASH();
	CleanPTLinks();

	Back_Scale_1();
	bool pv_analysis_isNeeded = Back_Scale_2();
	Back_Scale_3();
	Back_Scale_4();

	if (pv_analysis_isNeeded) {
		CleanPvTable();
		POINT_VECTOR_ANALYSIS();
	}


	CleanPTLinks();

	return 0; 
}




void* bucketsEx;


template<typename T, typename I = int>
void initializeBuckets(const size_t nElem) {
	numBuckets = nElem + 63;
	bucketsEx = calloc(numBuckets, sizeof(BucketEx<T, I>));
}

template<typename T, typename I = int>
void addBucketsEx(T val, const I index, uint64_t(*hash)(I)) {
	uint64_t bucketNum = hash(index);

	BucketEx<T>& bucket = ((BucketEx<T>*)bucketsEx)[bucketNum];
	if (bucket.allocatedSize == bucket.size) {
		bucket.Reallocate();
	}

	if (bucket.ptrEntries != NULL) {
		auto& entry = bucket.ptrEntries[bucket.size++];
		entry.entry = val;
		entry.index = index;
		entry.processed = 0;
	}
}

template<typename T, typename I>
void populateBucketsEx(T ar[], const size_t nElem, uint64_t(*hash)(I)) {
	initializeBuckets<T, I>(nElem);

	for (int j = 0; j < nElem; ++j) {
		addBucketsEx((T)ar[j], (I)j, hash);
	}
}

template<typename T, typename I = int>
bool lookupHashEx(I val, uint64_t(*hash)(I)) {
	BucketEx<T>& bucket = *(((BucketEx<T>*)bucketsEx) + hash(val));
	return bucket.lookupEx(val);
}

template<typename T, typename I = int>
BucketEx<T>::BucketEntry* lookupHashEntryEx(I index, uint64_t(*hash)(I)) {
	BucketEx<T>& bucket = *(((BucketEx<T>*)bucketsEx) + hash(index));
	return bucket.lookupIndex(index);
}

template<typename T, typename I = int>
BucketEx<T>::BucketEntry* lookupHashNextEntryEx(I index, typename BucketEx<T>::BucketEntry* ptr, uint64_t(*hash)(I)) {
	BucketEx<T>& bucket = *(((BucketEx<T>*)bucketsEx) + hash(index));
	return bucket.lookupIndexNext(index, ptr);
}


void populateBuckets(PtBead_Ptr ar[], const size_t nElem, uint64_t(*hash)(int)) {
	populateBucketsEx(ar, nElem, hash);
}



BucketEx<PtBead_Ptr>::BucketEntry* lookupHashEntry(int index, uint64_t(*hash)(int)) {
	return lookupHashEntryEx<PtBead_Ptr, int>(index, hash);
}

BucketEx<PtBead_Ptr>::BucketEntry* lookupHashNextEntry(int index, BucketEx<PtBead_Ptr>::BucketEntry* ptr, uint64_t(*hash)(int)) {
	return lookupHashNextEntryEx<PtBead_Ptr, int>(index, ptr, hash);
}


void addBucketsEntry(PtBead_Ptr val, const int index, uint64_t(*hash)(int)) {
	addBucketsEx(val, index, hash);
}




int numBuckets;




static uint64_t hashFunc(int x) {
	int64_t key = (23 * (int64_t)x + 103 * (int64_t)x + 293 * (int64_t)x + 397 * (int64_t)x) % numBuckets;
	if (key < 0) {
		key += numBuckets;
	}
	return key;
}




bool MergePoints(const long s_factor) {
	size_t NXcells = ZPTLIM;
	size_t NYcells = ZPTLIM;

	long XCellSize = (PXMAX - PXMIN) / (int)NXcells;
	long YCellSize = (PYMAX - PYMIN) / (int)NYcells;
	if (XCellSize == 0) {
		XCellSize = 1;
	}
	if (YCellSize == 0) {
		YCellSize = 1;
	}

	size_t NHashEntries = NXcells * NYcells;
	if (NHashEntries > 1000) {
		NHashEntries = 1000;
	}

	initializeBuckets<PtBead_Ptr, int>(NHashEntries);

	for (int p = 1; p < ZPTLIM; ++p) {
		auto pt = GetPtBead(p);

		int i = pt->ZPTCO1 / XCellSize; 
		int j = pt->ZPTCO2 / YCellSize;

		addBucketsEntry(pt, i * NXcells + j, hashFunc);
	}

	const long maxManhDistance = ((1 << s_factor) >> 1);
	//const long maxManhDistance = std::pow((1 << s_factor) >> 1, 2);
	//const long maxManhDistance = std::pow((1 << s_factor), 2);

	auto calcManhDistance = [&](double x1, double y1, double x2, double y2) -> double {
		return std::max(std::abs(x1 - x2), std::abs(y1 - y2));
		//return std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2);
	};


	struct KruskalEdge {
		PtBead* v;
		PtBead* w;
		long manhDistance;
	};

	std::vector<KruskalEdge> edges; 

	for (int i = 0; i < NXcells; ++i) {
		for (int j = 0; j < NYcells; ++j) {

			bool lookRight = (size_t(i) + 1) < NXcells;
			bool lookDown = (size_t(j) + 1) < NYcells;

			int cellNumber = i * NXcells + j;

			auto entry = lookupHashEntry(cellNumber, hashFunc);
			while (entry != nullptr) {
				if (entry->processed == 0) {
					entry->processed = 1;

					auto pt = entry->entry;

					int neighboringCells[] = { cellNumber, lookRight ? cellNumber + 1 : -1, lookDown ? cellNumber + NXcells : -1, lookRight && lookDown ? cellNumber + NXcells + 1 : -1 };

					for (auto searchCellNumber : neighboringCells) {
						if (searchCellNumber > -1) {
							auto searchEntry = lookupHashEntry(searchCellNumber, hashFunc);
							while (searchEntry != nullptr) {
								if (searchEntry->processed == 0) {
									auto searchPt = searchEntry->entry;
									auto manhDistance = calcManhDistance(pt->ZPTCO1, pt->ZPTCO2, searchPt->ZPTCO1, searchPt->ZPTCO2);
									if (manhDistance && manhDistance <= maxManhDistance) {
										edges.push_back({ pt, searchPt, (long)manhDistance });
									}
								}

								searchEntry = lookupHashNextEntry(searchCellNumber, searchEntry, hashFunc);
							}
						}
					}
				}

				entry = lookupHashNextEntry(cellNumber, entry, hashFunc);
			}
		}
	}

	if (edges.size() == 0) {
		return true;
	}




	struct customLess {
		bool operator()(const KruskalEdge& a, const KruskalEdge& b) const { return a.manhDistance < b.manhDistance; }
	};

	auto quickSort = [&]() {
		std::sort(edges.begin(), edges.end(), customLess());
	};

	quickSort();




	struct UnionNode {
		int t; // target, it is the parent of the node
		int w; // it is a rank of the node

		int f; // flag whether it was merged

		double avg_x;
		double avg_y;
	};

	std::vector<UnionNode> unionNodes(ZPTLIM); 
	std::queue<int> unionFindQueue;

	for (int p = 1; p < ZPTLIM; ++p) {
		auto pt = GetPtBead(p);

		unionNodes[p].t = p;
		unionNodes[p].w = 0;
		unionNodes[p].f = 0;

		pt->ZPTAux = (PtBead_Ptr)p;

		unionNodes[p].avg_x = pt->ZPTCO1;
		unionNodes[p].avg_y = pt->ZPTCO2;
	}



	auto unionFind = [&](int a) {
		int p = unionNodes[a].t;
		while (p != a) {
			int x = a;
			a = p;
			p = unionNodes[p].t;
			if (p != a) {
				unionFindQueue.push(x);
			}
			else {
				break;
			}
		}
		while (!unionFindQueue.empty()) {
			int x = unionFindQueue.front();
			unionFindQueue.pop();

			unionNodes[x].t = p;
		}

		return p;
	};

	auto unionJoin = [&](UnionNode& s, UnionNode& t) -> UnionNode& {
		if (s.w > t.w) {
			t.t = s.t;
			return t;
		}
		else if (s.w == t.w) {
			++s.w;
			t.t = s.t;
			return t;
		}

		s.t = t.t;
		return s;
	};


	bool isDone = true;


	for (auto& edge : edges) {
		int s_p = unionFind((size_t)edge.v->ZPTAux);
		int t_p = unionFind((size_t)edge.w->ZPTAux);
		if (s_p == t_p) {
			continue;
		}

		auto& s = unionNodes[s_p];
		auto& t = unionNodes[t_p];

		double manhDistance = (double)calcManhDistance(s.avg_x, s.avg_y, t.avg_x, t.avg_y);
		if (manhDistance <= maxManhDistance) {
			auto r = unionJoin(s, t);

			r.avg_x = (s.avg_x + t.avg_x) / 2;
			r.avg_y = (s.avg_y + t.avg_y) / 2;

			r.f = 1;
			s.f = 1;
			t.f = 1;
		}
		else {
			isDone = false;
		}
	}

	for (int p = 1; p < ZPTLIM; ++p) {
		auto pt = GetPtBead(p);
		pt->ZPTAux = 0;

		if (unionNodes[p].f == 0) {
			continue;
		}

		auto& clusterRoot = unionNodes[unionFind(p)];

		pt->ZPTCO1 = std::floor(clusterRoot.avg_x + 0.5);
		pt->ZPTCO2 = std::floor(clusterRoot.avg_y + 0.5);

		//if (clusterRoot.t != p) {
		//	pt->ZPTAux = GetPtBead(clusterRoot.t);
		//}
	}


	return isDone;
}
