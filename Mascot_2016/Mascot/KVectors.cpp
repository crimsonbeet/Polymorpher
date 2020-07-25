
#include "stdafx.h"

#include "KVectors.h"




KWinPoint::KWinPoint(): _x(0), _y(0) { 
} 

KWinPoint::KWinPoint(double x, double y): _x(x), _y(y) { 
} 

KWinPoint::KWinPoint(const IWSIPoint& wsipoint, double inv_ratio) { 
	_x = (double)(wsipoint.GetX() * inv_ratio); 
	_y = (double)(wsipoint.GetY() * inv_ratio); 
} 
bool KWinPoint::operator == (const KWinPoint& other) const { 
	return _x == other._x && _y == other._y; 
} 
double KWinPoint::Dot(const KWinPoint& p) { 
	return _x * p._x + _y * p._y; 
} 




KWinVector::KWinVector(): KWinPoint() { 
} 
KWinVector::KWinVector(const KWinPoint& point): KWinPoint(point) { 
} 
KWinVector::KWinVector(const IWSIPoint& wsipoint): KWinPoint(wsipoint) { 
} 
double KWinVector::Normalize() { 
	double norm2 = _x * _x + _y * _y; 
	if(norm2) { 
		double norm = sqrt((double)norm2); 
		_x = _x / norm; 
		_y = _y / norm; 
		return norm; 
	} 
	return 0; 
} 
KWinVector KWinVector::PerpendicularClockwise() { 
	return KWinVector(KWinPoint(_y, -_x)); 
} 
KWinVector KWinVector::PerpendicularCounterClockwise() { 
	return KWinVector(KWinPoint(-_y, _x)); 
} 



KWinLineImpl::KWinLineImpl(): _host(0) { 
} 


BOOL KWinLineExpl::Intersect(const KWinLineImpl& impl, KWinPoint& point) const { 
	BOOL rc = FALSE; 
	double d = (impl._n._x) * _v._x + (impl._n._y) * _v._y; 
	if(d != 0) { 
		double t = (impl._n._x * _p._x + impl._n._y * _p._y - impl._c) / d; 

		point._x = _p._x - _v._x * t; 
		point._y = _p._y - _v._y * t; 
		rc = TRUE; 
	} 
	return rc; 
} 

KWinLineExpl::KWinLineExpl(): _host(0) { 
} 




KWinRect::KWinRect(): _host(0) { 
	_x[0] = _y[0] = std::numeric_limits<double>::max(); 
	_x[1] = _y[1] = std::numeric_limits<double>::min(); 
} 

BOOL KWinRect::IsValid() const { 
	return _x[0] < _x[1] && _y[0] < _y[1]; 
} 

std::vector<KWinPoint> KWinRect::Intersect(const KWinLineExpl& line) const { 
	KWinPoint r_points[2]; 
	int r_cnt = 0; 

//	1. Check vertical sides. 
	if(line._v._x != 0) { // not vertical 
		for(int j = 0; j < 2; ++j) { 
			r_points[r_cnt]._x = _x[j]; 
			r_points[r_cnt]._y = line._p._y - line._v._y * (line._p._x - _x[j]) / line._v._x; 
			if(r_points[r_cnt]._y < _y[0]) { 
			} 
			else
			if(r_points[r_cnt]._y > _y[1]) { 
			} 
			else { 
				++r_cnt; 
			} 
		} 
	} 
//	2. Check horizontal sides. 
	if(line._v._y != 0) { // not horizontal 
		for(int j = 0; r_cnt < 2 && j < 2; ++j) { 
			r_points[r_cnt]._y = _y[j]; 
			r_points[r_cnt]._x = line._p._x - line._v._x * (line._p._y - _y[j]) / line._v._y; 
			if(r_points[r_cnt]._x < _x[0]) { 
			} 
			else
			if(r_points[r_cnt]._x > _x[1]) { 
			} 
			else 
			if(r_cnt == 1 && r_points[0] == r_points[1]) { 
			} 
			else { 
				++r_cnt; 
			} 
		} 
	} 
	return std::vector<KWinPoint>(r_points, r_points + r_cnt); 
} 

KWinRect KWinRect::Intersect(const KWinRect& rect) const { 
	return KWinRect(); 
} 




KWSIPoint::KWSIPoint(): _x(0), _y(0) { 
} 
KWSIPoint::KWSIPoint(double x, double y): _x(x), _y(y) { 
} 
KWSIPoint::KWSIPoint(const KWSIPoint& point): _x(point._x), _y(point._y) { 
} 
bool KWSIPoint::operator == (const KWSIPoint& other) const { 
	return _x == other._x && _y == other._y; 
} 
KWinRect KWSIPoint::Box() const { 
	KWinRect box; 
	box._x[0] = box._x[1] = _x; 
	box._y[0] = box._y[1] = _x; 
	return box; 
} 

double KWSIPoint::GetX() const { 
	return _x; 
} 
double KWSIPoint::GetY() const { 
	return _y; 
} 
COLOREDVoidPointer KWSIPoint::Intersect(COLOREDVoidPointer) { 
	return COLOREDVoidPointer(); 
} 
std::vector<COLOREDVoidPointer> KWSIPoint::Perform(std::multimap<std::string, COLOREDVoidPointer>& meta_map) { 
	std::vector<COLOREDVoidPointer> res; 
	res.push_back(COLOREDVoidPointer(new KWSIPoint(*this))); 
	return res; 
} 
void KWSIPoint::Draw(const KWinDrawParams& params) { 
//	std::cout << '{' << _x << ',' << _y << '}' << std::endl; 

	POINT pt; 

	GDIPoint(pt, params._x_ratio, params._y_ratio); 

	HPEN old_pen = (HPEN)SelectObject(params._hdc, GetStockObject(DC_PEN)); 
	SetDCPenColor(params._hdc, RGB(128, 128, 128)); 

	MoveToEx(params._hdc, pt.x, pt.y, NULL); 
	AngleArc(params._hdc, pt.x, pt.y, (DWORD)(params._pen_width * 2 + 0.5), 0, 360); 

/*	int pen_width = (int)(params._pen_width * 4 + 0.5); 

	POINT box_pt[2]; 
	box_pt[0].x = pt.x - pen_width; 
	box_pt[1].x = pt.x + pen_width; 
	box_pt[0].y = pt.y - pen_width; 
	box_pt[1].y = pt.y + pen_width; 

	MoveToEx(params._hdc, pt.x, pt.y, NULL); 
	ArcTo(params._hdc, box_pt[0].x, box_pt[0].y, box_pt[1].x, box_pt[1].y, box_pt[1].x, pt.y, box_pt[1].x, pt.y); */

	SelectObject(params._hdc, old_pen); 
} 




KWSIPointArray::KWSIPointArray() { 
} 
KWSIPointArray::KWSIPointArray(const std::vector<KWinPoint>& points) { 
	std::vector<KWinPoint>::const_iterator iter = points.begin(); 
	while(iter != points.end()) { 
		_points.push_back(KWSIPoint((*iter)._x, (*iter)._y)); 
		++iter; 
	} 
} 

KWinRect KWSIPointArray::Box() const { 
	KWinRect box; 
	std::vector<KWSIPoint>::const_iterator it = _points.begin(); 
	while(it < _points.end()) { 
		if((*it)._x < box._x[0]) { 
			box._x[0] = (*it)._x; 
		} 
		if((*it)._x > box._x[1]) { 
			box._x[1] = (*it)._x; 
		} 
		if((*it)._y < box._y[0]) { 
			box._y[0] = (*it)._y; 
		} 
		if((*it)._y > box._y[1]) { 
			box._y[1] = (*it)._y; 
		} 
		++it; 
	} 
	return box; 
} 

size_t KWSIPointArray::Count() const { 
	return _points.size(); 
} 
COLOREDVoidPointer KWSIPointArray::Intersect(COLOREDVoidPointer) { 
	return COLOREDVoidPointer(); 
} 
std::vector<COLOREDVoidPointer> KWSIPointArray::Perform(std::multimap<std::string, COLOREDVoidPointer>& meta_map) { 
	std::vector<COLOREDVoidPointer> res; 
	res.push_back(COLOREDVoidPointer(new KWSIPointArray(*this))); 
	return res; 
} 
void KWSIPointArray::Draw(const KWinDrawParams& params) { 
	POINT pt; 
	std::vector<KWSIPoint>::iterator it = _points.begin(); 
	int x = 0; 
	if(it < _points.end()) { 
		(*it).Draw(params); 
		(*it).GDIPoint(pt, params._x_ratio, params._y_ratio); 
		MoveToEx(params._hdc, pt.x, pt.y, NULL); 
		++it; 
	} 
	while(it < _points.end()) { 
//		if(x++ == 0) { 
			(*it).Draw(params); 
			MoveToEx(params._hdc, pt.x, pt.y, NULL); 
//		} 
		(*it).GDIPoint(pt, params._x_ratio, params._y_ratio); 
		LineTo(params._hdc, pt.x, pt.y); 
		++it; 
	} 
} 




void KWSILine::BuildImplicit(const KWSIPoint& norm/*normal vector*/, const KWSIPoint& point/*point on the line*/) { 
	_impl._n = norm; 
	_expl._p = point; 

	_impl._c = (&norm == &point? _impl._n.Normalize(): _impl._n.Dot(_expl._p)); 
	_expl._v = _impl._n.PerpendicularClockwise(); 

	_expl._host = this; 
	_impl._host = this; 
} 

void KWSILine::BuildExplicit(const KWSIPoint& covec/*vector along the line*/, const KWSIPoint& point/*point on the line*/) { 
	_expl._v = covec; 
	_expl._p = point; 

	_expl._v.Normalize(); 

	_impl._n = _expl._v.PerpendicularCounterClockwise(); 
	_impl._c = _impl._n.Dot(_expl._p); 

	_expl._host = this; 
	_impl._host = this; 
} 

const KWinLineImpl& KWSILine::GetImplicit() const { 
	return _impl; 
} 
const KWinLineExpl& KWSILine::GetExplicit() const { 
	return _expl; 
} 
KWinRect KWSILine::Box() const { 
	return KWinRect(); 
} 

COLOREDVoidPointer KWSILine::Intersect(COLOREDVoidPointer obj) { 
	switch(obj._ptr_type) { 
		case obj.PVOIDKWSIRect: 
			return KWSIRectPtr(obj)->Intersect(COLOREDVoidPointer(this)); 
		case obj.PVOIDKWSILine: 
			KWinPoint point; 
			if(_expl.Intersect(KWSILinePtr(obj)->_impl, point)) { 
				return COLOREDVoidPointer(new KWSIPoint(point._x, point._y)); 
			} 
			break; 
	} 
	return COLOREDVoidPointer(); 
} 

std::vector<COLOREDVoidPointer> KWSILine::Perform(std::multimap<std::string, COLOREDVoidPointer>& meta_map) { 
	std::vector<COLOREDVoidPointer> res; 
	res.push_back(COLOREDVoidPointer(new KWSILine(*this))); 
	return res; 
} 

void KWSILine::Draw(const KWinDrawParams& params) { 
	std::vector<KWinPoint> line = params._box.Intersect(GetExplicit()); 
	if(line.size() != 2) { 
		return; 
	} 

//	std::cout << '{' << line[0]._x << ',' << line[0]._y << '}' << '{' << line[1]._x << ',' << line[1]._y << '}' << std::endl; 

	POINT pt; 

	line[0].GDIPoint(pt, params._x_ratio, params._y_ratio); 
	MoveToEx(params._hdc, pt.x, pt.y, NULL); 

	line[1].GDIPoint(pt, params._x_ratio, params._y_ratio); 
	LineTo(params._hdc, pt.x, pt.y); 

} 


void KWSIRect::BuildExplicit(const KWSIPoint& pointA, const KWSIPoint& pointB) { 
	KWinPoint point1(pointA); 
	KWinPoint point2(pointB); 

	if(point1._x < point2._x) { 
		_rect._x[0] = point1._x; 
		_rect._x[1] = point2._x; 
	} 
	else { 
		_rect._x[0] = point2._x; 
		_rect._x[1] = point1._x; 
	} 
	if(point1._y < point2._y) { 
		_rect._y[0] = point1._y; 
		_rect._y[1] = point2._y; 
	} 
	else { 
		_rect._y[0] = point2._y; 
		_rect._y[1] = point1._y; 
	} 
		
	_rect._host = this; 
} 

const KWinRect& KWSIRect::GetRect() { 
	return _rect; 
} 
KWinRect KWSIRect::Box() const { 
	return _rect; 
} 

COLOREDVoidPointer KWSIRect::Intersect(COLOREDVoidPointer obj) { 
	return COLOREDVoidPointer(); 
} 

std::vector<COLOREDVoidPointer> KWSIRect::Perform(std::multimap<std::string, COLOREDVoidPointer>& meta_map) { 
	std::vector<COLOREDVoidPointer> res; 
	res.push_back(COLOREDVoidPointer(new KWSIRect(*this))); 
	return res; 
} 

void KWSIRect::Draw(const KWinDrawParams& params) { 
} 


COLOREDVoidPointer KWSIIntersect::Intersect(COLOREDVoidPointer obj) { 
	return COLOREDVoidPointer(); 
} 

KWinRect KWSIIntersect::Box() const { 
	return KWinRect(); 
} 

std::vector<COLOREDVoidPointer> KWSIIntersect::Perform(std::multimap<std::string, COLOREDVoidPointer>& meta_map) { 
	std::vector<COLOREDVoidPointer> res; 

	std::multimap<std::string, COLOREDVoidPointer>::iterator iter1 = meta_map.find(_item[0]); 
	while(iter1 != meta_map.end()) { 
		IWSIGraphObjectPtr obj1 = IWSIGraphObjectPtr((*iter1).second); 
		if(obj1->_id == _item[0]) { 
			std::multimap<std::string, COLOREDVoidPointer>::iterator iter2 = meta_map.find(_item[1]); 
			while(iter2 != meta_map.end()) { 
				IWSIGraphObjectPtr obj2 = IWSIGraphObjectPtr((*iter2).second); 
				if(obj2->_id == _item[1]) { 
					res.push_back(obj1->Intersect((*iter2).second)); 
					++iter2; 
				}
				else { 
					iter2 = meta_map.end(); 
				} 
			} 
			++iter1; 
		} 
		else { 
			iter1 = meta_map.end(); 
		} 
	} 
	return res; 
} 

