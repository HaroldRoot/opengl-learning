// 三维点类型 point
typedef struct Point3D {
    double x, y, z;
    Point3D(double a = 0.0, double b = 0.0, double c = 0.0) {
        x = a, y = b;
        z = c;
    }
} point3D;
// 二维点类型 point
// typedef struct Point {
//	int x, y;
//	Point(int a = 0, int b = 0)	{
//		x = a;
//		y = b;
//	}
// } point;
// 矩形类型 rect
// typedef struct Rectangle{
//	float w_xmin,w_ymin;
//	float w_xmax,w_yman;
//	Rectangle(float xmin = 0.0, float ymin = 0.0,float xmax=0.0,float yman=0.0){
//		w_xmin = xmin;	w_ymin = ymin;
//		w_xmax = xmax;	w_yman = yman;
//	}
// }rect;
// 颜色类型 color
typedef struct Color {
    int r, g, b;
    Color(int red = 0, int green = 0, int blue = 0) {
        r = red;
        g = green;
        b = blue;
    }
} color;