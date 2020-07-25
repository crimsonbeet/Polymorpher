#ifndef KVectorsH 
#define KVectorsH 

// Each drawing is confined within a limiting Box. 
// Each drawing is directed into memdc, supported by DIBSection. 
// Aspect Ratio of limiting Box is preserved in dimesions of memdc. 
// Dimensions of memdc: 
/* 
	A = max(width, height) of Windows Client Rectangle. 
	B = max(width, height) of Limiting Box. 
	Ratio = A / B; 
	memdc.width = Box.width * Ratio; 
	memdc.height = Box.height * Ratio; 
*/ 
// Dimensions of memdc define device units. 
// Logical Units (Page Units) are defined either 
// by limiting Box, or 
// by memdc whichever is bigger. 
// 
// When memdc dimensions are used for logical units, 
// then the graphic values of KWin object have to be multiplied by above Ratio when calling GDI, 
// because 
// KWin objects use <double float> as internal type to store all values and 
// Whenever GDI call must be made, the precision can be lost. 
// then use 
// SetViewportExtEx(memdc, 1, 1, NULL); 
// SetwindowExtEx(memdc, 1, 1, NULL); 
//
// When dimensions of limiting Box are used for logical units, 
// then use 
// SetViewportExtEx(memdc, 1, 1, NULL); 
// SetwindowExtEx(memdc, (int)1/Ratio, (int)1/Ratio, NULL); 


// >< $ Box >< Point ... >< Point ... $ 
// >< $ LineImpl Id=A >< Point ... $ 
// >< $ RectExpl Id=B >< Point ... >< Point ... $ 
// >< $ IntersectPoints Id=C Line=A Rect=B $ 
// or
// >< $ IntersectPoints Id=C Rect=B >< LineImpl ... $ 


#ifdef WSI_NAMESPACE
#undef WSI_NAMESPACE
#endif
#ifdef autocreateserialization_not_necessaryH
#undef autocreateserialization_not_necessaryH
#endif

#define WSI_NAMESPACE KVectors



#include "WSIClassFactory.h" 



struct KWinPoint; 
struct KWinVector; 
struct KWinLineImpl; 
struct KWinLineExpl; 
struct KWinRect; 

struct KWinDrawParams; 



class KWSIPoint; 
typedef KWSIPoint *KWSIPointPtr; 

class KWSILine; 
typedef KWSILine *KWSILinePtr; 

class KWSIRect; 
typedef KWSIRect *KWSIRectPtr; 

class KWSIPointArray; 
typedef KWSIPointArray *KWSIPointArrayPtr; 



struct IWSIGraphObject; 
typedef IWSIGraphObject *IWSIGraphObjectPtr; 




class PointExpl;
typedef PointExpl *PointExplPtr; 

class LineExpl; 
typedef LineExpl *LineExplPtr; 

class LineImpl; 
typedef LineImpl *LineImplPtr; 

class LineImpl2; 
typedef LineImpl2 *LineImpl2Ptr; 

class LineImpl3; 
typedef LineImpl3 *LineImpl3Ptr; 

class RectExpl; 
typedef RectExpl *RectExplPtr; 

class PointExplVec; 
typedef PointExplVec *PointExplVecPtr; 




struct COLOREDVoidPointer { 
	enum { 
		PVOIDNone, 
/*		PVOIDPointExpl, 
		PVOIDLineExpl, 
		PVOIDLineImpl, 
		PVOIDLineImpl2, 
		PVOIDLineImpl3, 
		PVOIDRectExpl, 
		PVOIDPointExplVec, */
		PVOIDKWSIPoint, 
		PVOIDKWSILine, 
		PVOIDKWSIRect, 
		PVOIDKWSIPointArray 
	}; 
	PVOID _ptr; 
	int _ptr_type; 
	COLOREDVoidPointer(): _ptr(NULL), _ptr_type(PVOIDNone) { 
	} 

	operator IWSIGraphObjectPtr() { 
		return _ptr_type != PVOIDNone? (IWSIGraphObjectPtr)_ptr: 0; 
	} 
//
/*	explicit COLOREDVoidPointer(PointExplPtr ptr): 
		_ptr(ptr), _ptr_type(PVOIDPointExpl) { 
	} 
	operator PointExplPtr() { 
		return _ptr_type == PVOIDPointExpl? (PointExplPtr)_ptr: 0; 
	} 
//
	explicit COLOREDVoidPointer(LineExplPtr ptr): 
		_ptr(ptr), _ptr_type(PVOIDLineExpl) { 
	} 
	operator LineExplPtr() { 
		return _ptr_type == PVOIDLineExpl? (LineExplPtr)_ptr: 0; 
	} 
//
	explicit COLOREDVoidPointer(LineImplPtr ptr): 
		_ptr(ptr), _ptr_type(PVOIDLineImpl) { 
	} 
	operator LineImplPtr() { 
		return _ptr_type == PVOIDLineImpl? (LineImplPtr)_ptr: 0; 
	} 
//
	explicit COLOREDVoidPointer(LineImpl2Ptr ptr): 
		_ptr(ptr), _ptr_type(PVOIDLineImpl2) { 
	} 
	operator LineImpl2Ptr() { 
		return _ptr_type == PVOIDLineImpl2? (LineImpl2Ptr)_ptr: 0; 
	} 
//
	explicit COLOREDVoidPointer(LineImpl3Ptr ptr): 
		_ptr(ptr), _ptr_type(PVOIDLineImpl3) { 
	} 
	operator LineImpl3Ptr() { 
		return _ptr_type == PVOIDLineImpl3? (LineImpl3Ptr)_ptr: 0; 
	} 
//
	explicit COLOREDVoidPointer(RectExplPtr ptr): 
		_ptr(ptr), _ptr_type(PVOIDRectExpl) { 
	} 
	operator RectExplPtr() { 
		return _ptr_type == PVOIDRectExpl? (RectExplPtr)_ptr: 0; 
	} 
//
	explicit COLOREDVoidPointer(PointExplVecPtr ptr): 
		_ptr(ptr), _ptr_type(PVOIDPointExplVec) { 
	} 
	operator PointExplVecPtr() { 
		return _ptr_type == PVOIDPointExplVec? (PointExplVecPtr)_ptr: 0; 
	} */
// 
	explicit COLOREDVoidPointer(KWSIPointPtr ptr): 
		_ptr(ptr), _ptr_type(PVOIDKWSIPoint) { 
	} 
	operator KWSIPointPtr() { 
		return _ptr_type == PVOIDKWSIPoint? (KWSIPointPtr)_ptr: 0; 
	} 
// 
	explicit COLOREDVoidPointer(KWSILinePtr ptr): 
		_ptr(ptr), _ptr_type(PVOIDKWSILine) { 
	} 
	operator KWSILinePtr() { 
		return _ptr_type == PVOIDKWSILine? (KWSILinePtr)_ptr: 0; 
	} 
// 
	explicit COLOREDVoidPointer(KWSIRectPtr ptr): 
		_ptr(ptr), _ptr_type(PVOIDKWSIRect) { 
	} 
	operator KWSIRectPtr() { 
		return _ptr_type == PVOIDKWSIRect? (KWSIRectPtr)_ptr: 0; 
	} 
// 
	explicit COLOREDVoidPointer(KWSIPointArrayPtr ptr): 
		_ptr(ptr), _ptr_type(PVOIDKWSIPointArray) { 
	} 
	operator KWSIPointArrayPtr() { 
		return _ptr_type == PVOIDKWSIPointArray? (KWSIPointArrayPtr)_ptr: 0; 
	} 
}; 





struct IWSIGraphObject { 
	std::string _id; 
	virtual KWinRect Box() const = 0; 
	virtual std::vector<COLOREDVoidPointer> Perform(std::multimap<std::string, COLOREDVoidPointer>& meta_map) = 0; 
	virtual COLOREDVoidPointer Intersect(COLOREDVoidPointer) = 0; 
	virtual void Draw(const KWinDrawParams& params) = 0; 
	virtual void Build() { 
	} 
	virtual ~IWSIGraphObject() { 
	} 
	IWSIGraphObject() { 
		memset(&brush, 0, sizeof(brush)); 
	} 

	virtual void SetBrush(long index) { 
		COLORREF color = RGB(255, 0, 0); // red
		switch(index) { 
		case 1: 
			color = RGB(127, 255, 0); // chartreuse - 0x7fff00
			break; 
		case 2: 
			color = RGB(0, 255, 255); // cyan - 0x00ffff
			break; 
		case 3: 
			color = RGB(255, 215, 0); // gold - 0xffd700
			break; 
		case 4: 
			color = RGB(255, 255, 240); // ivory - 0xfffff0
			break; 
		case 5: 
			color = RGB(255, 255, 240); // purple - 0xff2dff
			break; 
		} 
		LOGBRUSH br = { BS_SOLID, color, 0 }; 
		brush = br; 
	} 

	LOGBRUSH brush; 
}; 

struct IWSIPoint: public IWSIGraphObject { 
	virtual double GetX() const = 0; 
	virtual double GetY() const = 0; 
}; 

struct IWSIPointArray: public IWSIGraphObject { 
	virtual size_t Count() const = 0; 
}; 

struct IWSILine: public IWSIGraphObject { 
	virtual const KWinLineImpl& GetImplicit() const = 0; 
	virtual const KWinLineExpl& GetExplicit() const = 0; 
}; 

struct IWSIRect: public IWSIGraphObject { 
	virtual const KWinRect& GetRect() = 0; 
}; 

struct IWSIIntersect: public IWSIGraphObject { 
}; 





struct KWinPoint { 
	double _x; 
	double _y; 

	KWinPoint(); 
	KWinPoint(double x, double y); 
	KWinPoint(const IWSIPoint& wsipoint, double inv_ratio = 1.0 /*use this ratio if pre-multiplication is neccessary*/); 

	bool operator == (const KWinPoint& other) const; 
	double Dot(const KWinPoint& p); 

	inline void GDIPoint(POINT& pt/*out*/, double x_ratio = 1.0, double y_ratio = 1.0) const { 
		pt.x = (int)(_x * x_ratio + 0.45); 
		pt.y = (int)(_y * y_ratio + 0.45); 
	} 
}; 

struct KWinVector: KWinPoint { 
	KWinVector(); 
	KWinVector(const KWinPoint& point); 
	KWinVector(const IWSIPoint& wsipoint); 
	double Normalize(); 
	KWinVector PerpendicularClockwise(); 
	KWinVector PerpendicularCounterClockwise(); 
}; 


struct KWinLineImpl { 
//	implicit (p * _n = c) 
	KWinVector _n; 
	double _c; 

	IWSILine *_host; 

	KWinLineImpl(); 
}; 

struct KWinLineExpl { 
//	explicit (p = _p + _v * t). 
	KWinPoint _p; 
	KWinVector _v; 

	IWSILine *_host; 

	KWinLineExpl(); 

	BOOL Intersect(const KWinLineImpl& impl/*in*/, KWinPoint& point/*out*/) const; 
}; 

struct KWinRect { 
	double _x[2]; // ordered. 
	double _y[2]; // ordered. 

	IWSIRect *_host; 

	KWinRect(); 

	BOOL IsValid() const; 

	std::vector<KWinPoint> Intersect(const KWinLineExpl& line) const; 
	KWinRect Intersect(const KWinRect& rect) const; 
}; 


struct KWinDrawParams { 
	HDC _hdc; 
	KWinRect _box; 
	double _x_ratio; 
	double _y_ratio; 
	double _pen_width; 
}; 









class KWSIPoint: public IWSIPoint { 
public: 
	double _x; 
	double _y; 
	inline void GDIPoint(POINT& pt/*out*/, double x_ratio = 1.0, double y_ratio = 1.0) const { 
		pt.x = (int)(_x * x_ratio + 0.45); 
		pt.y = (int)(_y * y_ratio + 0.45); 
	} 
	KWinRect Box() const; 
	KWSIPoint(); 
	KWSIPoint(double x, double y); 
	KWSIPoint(const KWSIPoint& point); 
	bool operator == (const KWSIPoint& other) const; 
	double GetX() const; 
	double GetY() const; 
	COLOREDVoidPointer Intersect(COLOREDVoidPointer); 
	std::vector<COLOREDVoidPointer> Perform(std::multimap<std::string, COLOREDVoidPointer>& meta_map); 
	void Draw(const KWinDrawParams& params); 
}; 



class KWSIPointArray: public IWSIPointArray { 
public: 
	std::vector<KWSIPoint> _points; 

	KWSIPointArray(); 
	KWSIPointArray(const std::vector<KWinPoint>& points); 

	KWinRect Box() const; 
	size_t Count() const; 
	COLOREDVoidPointer Intersect(COLOREDVoidPointer); 
	std::vector<COLOREDVoidPointer> Perform(std::multimap<std::string, COLOREDVoidPointer>& meta_map); 
	void Draw(const KWinDrawParams& params); 
}; 



class KWSILine: public IWSILine { 
protected: 
	KWinLineImpl _impl; 
	KWinLineExpl _expl; 

	void BuildImplicit(const KWSIPoint& norm/*normal vector*/, const KWSIPoint& point/*point on the line*/); 
	void BuildExplicit(const KWSIPoint& covec/*vector along the line*/, const KWSIPoint& point/*point on the line*/); 

public: 
	KWinRect Box() const; 
	const KWinLineImpl& GetImplicit() const; 
	const KWinLineExpl& GetExplicit() const; 
	COLOREDVoidPointer Intersect(COLOREDVoidPointer obj); 
	std::vector<COLOREDVoidPointer> Perform(std::multimap<std::string, COLOREDVoidPointer>& meta_map); 
	void Draw(const KWinDrawParams& params); 
}; 



class KWSIRect: public IWSIRect { 
protected: 
	KWinRect _rect; 

	void BuildExplicit(const KWSIPoint& pointA, const KWSIPoint& pointB); 

public: 
	KWinRect Box() const; 
	const KWinRect& GetRect(); 
	COLOREDVoidPointer Intersect(COLOREDVoidPointer); 
	std::vector<COLOREDVoidPointer> Perform(std::multimap<std::string, COLOREDVoidPointer>& meta_map); 
	void Draw(const KWinDrawParams& params); 
}; 



class KWSIIntersect: public IWSIGraphObject { 
protected: 
	std::string _item[2]; 

public: 
	KWinRect Box() const; 
	COLOREDVoidPointer Intersect(COLOREDVoidPointer); 
	std::vector<COLOREDVoidPointer> Perform(std::multimap<std::string, COLOREDVoidPointer>& meta_map); 
	void Draw(const KWinDrawParams& params) { 
	} 
}; 






class PointExpl: public KWSIPoint { 
public: 
	void Build() { 
	} 
}; 

BEGIN_WSI_SERIALIZATION_OBJECT(PointExpl)

CONTAINS_INHERITED_FLAT_MEMBER(IWSIGraphObject, _id, Id)
CONTAINS_INHERITED_FLAT_MEMBER(KWSIPoint, _x, X)
CONTAINS_INHERITED_FLAT_MEMBER(KWSIPoint, _y, Y)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(PointExpl)



class PointExplVec: public KWSIPointArray { 
public: 
	std::vector<PointExpl> _points_expl; 

	void Build() { 
		for(size_t j = 0; j < _points_expl.size(); ++j) { 
			_points.push_back(KWSIPoint(_points_expl[j])); 
		} 
	} 
}; 

BEGIN_WSI_SERIALIZATION_OBJECT(PointExplVec)

CONTAINS_INHERITED_FLAT_MEMBER(IWSIGraphObject, _id, Id)
CONTAINS_OBJECT_MEMBER(_points_expl, Point)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(PointExplVec)



class LineExpl: public KWSILine { 
public: 
	PointExpl _points[2]; // Line through two points. 

	void Build() { 
		PointExpl point = _points[0]; 
		PointExpl covec = _points[1]; 
		covec._x -= point._x; 
		covec._y -= point._y; 

		BuildExplicit(covec, point); 
	} 
}; 

BEGIN_WSI_SERIALIZATION_OBJECT(LineExpl)

CONTAINS_INHERITED_FLAT_MEMBER(IWSIGraphObject, _id, Id)
CONTAINS_OBJECT_MEMBER(_points, Point)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(LineExpl)



class LineImpl: public KWSILine { 
public: 
	PointExpl _point; // Line through point; point defines normal vector. 

	void Build() { 
		BuildImplicit(_point, _point); 
	} 
}; 

BEGIN_WSI_SERIALIZATION_OBJECT(LineImpl)

CONTAINS_INHERITED_FLAT_MEMBER(IWSIGraphObject, _id, Id)
CONTAINS_OBJECT_MEMBER(_point, Point)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(LineImpl)



class LineImpl2: public KWSILine { 
public: 
	double _angle_rad; // Angle defiines normal unit vector. 
	double _dist; // Distance defines point along normal vector. 

	void Build() { 
		KWSIPoint norm(cos(_angle_rad), sin(_angle_rad)); 

		BuildImplicit(norm, KWSIPoint(norm._x * _dist, norm._y * _dist)); 
	} 
}; 

BEGIN_WSI_SERIALIZATION_OBJECT(LineImpl2)

CONTAINS_INHERITED_FLAT_MEMBER(IWSIGraphObject, _id, Id)
CONTAINS_FLAT_MEMBER(_angle_rad, Angle)
CONTAINS_FLAT_MEMBER(_dist, Dist)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(LineImpl2)



class LineImpl3: public KWSILine { 
public: 
	double _angle_rad; // Angle defiines normal unit vector. 
	PointExpl _point; // Line through point. 

	void Build() { 
		KWSIPoint norm(cos(_angle_rad), sin(_angle_rad)); 

		BuildImplicit(norm, _point); 
	} 
}; 

BEGIN_WSI_SERIALIZATION_OBJECT(LineImpl3)

CONTAINS_INHERITED_FLAT_MEMBER(IWSIGraphObject, _id, Id)
CONTAINS_FLAT_MEMBER(_angle_rad, Angle)
CONTAINS_OBJECT_MEMBER(_point, Point)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(LineImpl3)



class RectExpl: public KWSIRect { 
public: 
	PointExpl _points[2]; 

	void Build() { 
		BuildExplicit(_points[0], _points[1]); 
	} 
	BOOL IsValid() const { 
		return _points[0]._x < _points[1]._x && _points[0]._y < _points[1]._y; 
	} 
	void MaxBox(const KWinRect& rect) { 
		if(rect.IsValid()) { 
			if(rect._x[0] < _points[0]._x) { 
				_points[0]._x = rect._x[0]; 
			} 
			if(rect._x[1] > _points[1]._x) { 
				_points[1]._x = rect._x[1]; 
			} 
			if(rect._y[0] < _points[0]._y) { 
				_points[0]._y = rect._y[0]; 
			} 
			if(rect._y[1] > _points[1]._y) { 
				_points[1]._y = rect._y[1]; 
			} 
		} 
	} 
	static RectExpl GetMaxMin() { 
		RectExpl r; 
		r._points[0]._x = r._points[0]._y = std::numeric_limits<double>::max(); 
		r._points[1]._x = r._points[1]._x = std::numeric_limits<double>::min();
		return r; 
	}
}; 

BEGIN_WSI_SERIALIZATION_OBJECT(RectExpl)

CONTAINS_INHERITED_FLAT_MEMBER(IWSIGraphObject, _id, Id)
CONTAINS_OBJECT_MEMBER(_points, Point)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(RectExpl)




class Intersect: public KWSIIntersect { 
public: 
	std::string _ref[2]; 

	void Build() { 
		_item[0].resize(0); 
		_item[1].resize(0); 
		if(_ref[0] != _ref[1]) { 
			if(_ref[0].size() && _ref[1].size()) { 
				_item[0] = _ref[0]; 
				_item[1] = _ref[1]; 
			} 
		} 
	} 
}; 

BEGIN_WSI_SERIALIZATION_OBJECT(Intersect)

CONTAINS_INHERITED_FLAT_MEMBER(IWSIGraphObject, _id, Id)
CONTAINS_FLAT_MEMBER(_ref, Ref)
END_WSI_SERIALIZATION_OBJECT()

AUTOCREATE_WSI_SERIALIZATION_OBJECT(Intersect)











class KWSIMetaFile: public wsi_gate { 
public: 
	std::string _name; 
	RectExpl _box; 
	double _y_ratio; 

//	_meta_entries contains all objects generated from input. 
//	each entry is identified by non unique id. 
//	different entries can be grouped by id. 
	std::multimap<std::string, COLOREDVoidPointer> _meta_map; 

//	_meta_entries contains all the input entries with their order preserved. 
//	each entry will be asked to perform an action against some object in _meta_map. 
//	an output of that action is COLOREDVoidPointer. 
//	each COLOREDVoidPointer will be placed back into _meta_map. 
	std::vector<IWSIGraphObject*> _meta_entries; 

	KWSIMetaFile(): _y_ratio(1) { 
	} 

	~KWSIMetaFile() { 
		Clear(); 
	} 

	void AddPtr(IWSIGraphObject *ptr, const std::string& id = std::string()) { 
		_meta_entries.push_back(ptr); 
	} 

	void AddPtr(const std::vector<COLOREDVoidPointer>& vec, const std::string& id = std::string()) { 
		std::vector<COLOREDVoidPointer>::const_iterator iter = vec.begin(); 
		while(iter != vec.end()) { 
			COLOREDVoidPointer ptr = (*iter); 
			if(ptr._ptr_type != ptr.PVOIDNone) { 
				_meta_map.insert(std::map<std::string, COLOREDVoidPointer>::value_type(id, ptr)); 
			} 
			++iter; 
		} 
	} 


	template<typename T> void _callback(T *obj) { 
		AddPtr(new T(*obj), obj->_id); 
	} 


	std::vector<IWsiConsumerBase*> _callbacks; 

	void InitializeMarshaller(IWSIMarshaller& marsh) { 
		_callbacks.push_back(CreateAPICallback<KWSIMetaFile, RectExpl>(&KWSIMetaFile::_callback, this, &marsh)); 
		_callbacks.push_back(CreateAPICallback<KWSIMetaFile, LineExpl>(&KWSIMetaFile::_callback, this, &marsh)); 
		_callbacks.push_back(CreateAPICallback<KWSIMetaFile, LineImpl>(&KWSIMetaFile::_callback, this, &marsh)); 
		_callbacks.push_back(CreateAPICallback<KWSIMetaFile, Intersect>(&KWSIMetaFile::_callback, this, &marsh)); 
	} 

	void Configure(const std::string& config) { 
		IXFunction<IWSIMarshaller, void> *pFunc = AllocXMemFunction(&KWSIMetaFile::InitializeMarshaller, this); 
		XConfigure(*this, config, pFunc); 
		std::for_each(_callbacks.begin(), _callbacks.end(), DestroyIWSICallable); 
		_callbacks.resize(0); 

		delete pFunc; 
	} 

	void Build() { 
		RectExpl max_box = RectExpl::GetMaxMin(); 
		bool max_box_isvalid = false; 

		std::vector<IWSIGraphObject*>::iterator iter = _meta_entries.begin(); 
		while(iter != _meta_entries.end()) { 
			(*iter)->Build(); 
			max_box.MaxBox((*iter)->Box()); 
			max_box_isvalid = true; 
			++iter; 
		} 

		if(!_box.IsValid()) { 
			_box = max_box; 
		} 

		_box.Build(); 

		if(max_box_isvalid) { 
			double dx = _box._points[1]._x - _box._points[0]._x; 
			double dy = _box._points[1]._y - _box._points[0]._y; 

			_y_ratio = 1; 
			if(dx >= 0 && dy >= 0) { 
				_y_ratio = dx / (dy * 2); 
			} 
		} 
	} 

	void Clear() { 
		size_t j; 
		size_t s;
		for(j = 0, s = _meta_entries.size(); j < s; ++j) { 
			delete _meta_entries[j]; 
		} 
		std::multimap<std::string, COLOREDVoidPointer>::iterator iter = _meta_map.begin(); 
		while(iter != _meta_map.end()) { 
			delete IWSIGraphObjectPtr((*iter++).second); 
		} 

		_meta_entries.clear(); 
		_meta_map.clear(); 
	} 

	void Perform() { 
		std::vector<IWSIGraphObject*>::iterator iter = _meta_entries.begin(); 
		while(iter != _meta_entries.end()) { 
			AddPtr((*iter)->Perform(_meta_map), (*iter)->_id); 
			++iter; 
		} 
	} 

	void Draw(const KWinDrawParams& params) { 
		std::multimap<std::string, COLOREDVoidPointer>::iterator iter = _meta_map.begin(); 
		while(iter != _meta_map.end()) { 
			IWSIGraphObjectPtr obj = IWSIGraphObjectPtr((*iter).second); 
			HPEN old_pen = NULL; 
			HPEN new_pen = NULL; 
			if(obj->brush.lbStyle != 0 || obj->brush.lbHatch != 0 || obj->brush.lbColor !=  RGB(0, 0, 0)) { 
				double pen_widht = params._pen_width; 
//				if(obj->brush.lbColor ==  RGB(255, 0, 0)) { 
//					pen_widht *= 2.5; 
//				} 
				new_pen = ExtCreatePen(PS_GEOMETRIC | PS_SOLID, pen_widht > 1? (int)(pen_widht + 0.5): 1, &obj->brush, 0, NULL); 
				old_pen = (HPEN)SelectObject(params._hdc, new_pen); 
			} 
			obj->Draw(params); 
			if(new_pen != NULL) { 
				SelectObject(params._hdc, old_pen); 
				DeleteObject(new_pen); 
			} 
			++iter; 
		} 
	} 
}; 


BEGIN_WSI_SERIALIZATION_OBJECT(KWSIMetaFile) 

CONTAINS_FLAT_MEMBER(_name, Name) 
CONTAINS_OBJECT_MEMBER(_box, Box) 
END_WSI_SERIALIZATION_OBJECT() 








#endif //KVectorsH 
