#include <bits/stdc++.h>
using namespace std;
#define pb push_back
/*
计算距离某个点x,y的最近点距离
 */
const double eps = 1e-8;
const double inf = 1e20;
const double pi = acos(-1.0);
//符号函数 减小精度误差
int sgn(double x){
	if(fabs(x)<eps) return 0;
	if(x<0) return -1;
	else return 1;
}
inline double sqr(double x){
	return x*x;
}
struct Point{
	double x,y;
	Point(){}
	Point(double _x,double _y){
		x = _x;
		y = _y;
	}
	bool operator == (Point b)const{
		return sgn(x-b.x) == 0 && sgn(y-b.y) == 0;
	}
	bool operator < (Point b)const{
		return sgn(x-b.x)== 0?sgn(y-b.y)<0:x<b.x;
	}
	Point operator -(const Point &b)const{
		return Point(x-b.x,y-b.y);
	}
//叉积
	double operator ^(const Point &b)const{
		return x*b.y - y*b.x;
	}
//点积
	double operator *(const Point &b)const{
		return x*b.x + y*b.y;
	}
//返回长度
	double len(){
		return hypot(x,y);//库函数
	}
	//返回长度的平方
	double len2(){
		return x*x + y*y;
	}
	//返回两点的距离
	double distance(Point p){
		return hypot(x-p.x,y-p.y);
	}
	Point operator +(const Point &b)const{
		return Point(x+b.x,y+b.y);
	}
	Point operator *(const double &k)const{
		return Point(x*k,y*k);
	}
	Point operator /(const double &k)const{
		return Point(x/k,y/k);
	}
	//化为长度为 r 的向量
	Point trunc(double r){
		double l = len();
		if(!sgn(l))return *this;
		r /= l;
		return Point(x*r,y*r);
	}
//逆时针旋转度
	Point rotleft(){
		return Point(-y,x);
	}
//顺时针旋转  度
	Point rotright(){
		return Point(y,-x);
	}
//绕着 p 点逆时针旋转 angle
	Point rotate(Point p,double angle){
	Point v = (*this) - p;
	double c = cos(angle), s = sin(angle);
	return Point(p.x + v.x*c - v.y*s,p.y + v.x*s + v.y*c);
	}
};
struct Line{
Point s,e;
	Line(){}
	Line(Point _s,Point _e){
		s = _s;
		e = _e;
	}
	bool operator ==(Line v){
		return (s == v.s)&&(e == v.e);
	}
//根据一个点和倾斜角 angle 确定直线,0<=angle<pi
	Line(Point p,double angle){
		s = p;
		if(sgn(angle-pi/2) == 0){
			e = (s + Point(0,1));
		}
		else{
			e = (s + Point(1,tan(angle)));
		}
	}
//求线段长度
	double length(){
		return s.distance(e);
	}
//点和直线关系
//1 在左侧
//2 在右侧
//3 在直线上
	int relation(Point p){
		int c = sgn((p-s)^(e-s));
		if(c < 0)return 1;
		else if(c > 0)return 2;
		else return 3;
	}
	// 点在线段上的判断
	bool pointonseg(Point p){
		return sgn((p-s)^(e-s)) == 0 && sgn((p-s)*(p-e)) <= 0;
	}
	//两向量平行 (对应直线平行或重合)
	bool parallel(Line v){
		return sgn((e-s)^(v.e-v.s)) == 0;
	}
	//直线和线段相交判断
	//-*this line -v seg
	//2 规范相交
	//1 非规范相交
	//0 不相交
	int linecrossseg(Line v){
		int d1 = sgn((e-s)^(v.s-s));
		int d2 = sgn((e-s)^(v.e-s));
		if((d1^d2)==-2) return 2;
		return d1==0||d2==0;
	}
	//两直线关系
	//0 平行
	//1 重合
	//2 相交
	int linecrossline(Line v){
		if((*this).parallel(v))
			return v.relation(s)==3;
		return 2;
	}
	//求两直线的交点
	//要保证两直线不平行或重合
	Point crosspoint(Line v){
		double a1 = (v.e-v.s)^(s-v.s);
		double a2 = (v.e-v.s)^(e-v.s);
		return Point((s.x*a2-e.x*a1)/(a2-a1),(s.y*a2-e.y*a1)/(a2-a1));
	}
	//点到直线的距离
	double dispointtoline(Point p){
		return fabs((p-s)^(e-s))/length();
	}
	//点到线段的距离
	double dispointtoseg(Point p){
		if(sgn((p-s)*(e-s))<0 || sgn((p-e)*(s-e))<0)
			return min(p.distance(s),p.distance(e));
		return dispointtoline(p);
	}
	//返回线段到线段的距离
	//前提是两线段不相交，相交距离就是 0 了
	double dissegtoseg(Line v){
		return min(min(dispointtoseg(v.s),dispointtoseg(v.e)),min(v.dispointtoseg(s),v.dispointtoseg(e)));
	}
	//返回点 p 在直线上的投影
	Point lineprog(Point p){
		return s + ( ((e-s)*((e-s)*(p-s)))/((e-s).len2()) );
	}
//返回点 p 关于直线的对称点
};
struct circle{
	Point p;//圆心
	double r;//半径
	circle(){}
	circle(Point _p,double _r){
		p = _p;
		r = _r;
	}
	circle(double x,double y,double _r){
		p = Point(x,y);
		r = _r;
	}
	//输出
	void output(){
		printf("%.2lf %.2lf %.2lf\n",p.x,p.y,r);
	}
	bool operator == (circle v){
		return (p==v.p) && sgn(r-v.r)==0;
	}
	bool operator < (circle v)const{
		return ((p<v.p)||((p==v.p)&&sgn(r-v.r)<0));
	}
	//直线和圆的关系
	//比较的是圆心到直线的距离和半径的关系
	int relationline(Line v){
		double dst = v.dispointtoline(p);
		if(sgn(dst-r) < 0)return 2;
		else if(sgn(dst-r) == 0)return 1;
		return 0;
	}
	//求直线和圆的交点，返回交点个数
	int pointcrossline(Line v,Point &p1,Point &p2){
		if(!(*this).relationline(v))return 0;
		Point a = v.lineprog(p);
		double d = v.dispointtoline(p);
		d = sqrt(r*r-d*d);
		if(sgn(d) == 0){
			p1 = a;
			p2 = a;
			return 1;
		}
		p1 = a + (v.e-v.s).trunc(d);
		p2 = a - (v.e-v.s).trunc(d);
		return 2;
	}
};
int angle_num,circle_num,line_num;
vector <Line> line_in_map;
vector <circle> circle_in_map;
double min_distacne[1024];
double angleOfRadar[1024];
void put_into_array(int index,double dist,Point temp,Point center){
	if(angleOfRadar[index]<360&&angleOfRadar[index]>180){
		if(sgn(temp.y-center.y)<0) min_distacne[index] = min(min_distacne[index],dist);//下方合法的交点
	}
	if(angleOfRadar[index]<180&&angleOfRadar[index]>0){
		if(sgn(temp.y-center.y)>0) min_distacne[index] = min(min_distacne[index],dist);//上方合法的交点
	}
	if(sgn(angleOfRadar[index]-180)==0){//水平方向朝左
		if(sgn(temp.x-center.x)<0) min_distacne[index] = min(min_distacne[index],dist);
	}
	if(sgn(angleOfRadar[index])==0){//水平方向朝右
		if(sgn(temp.x-center.x)>0) min_distacne[index] = min(min_distacne[index],dist);//右边的交点
	}
}
void get_min_with_segment(Line myradar,int index,Point center){//myradar是我们从雷达中心引出的射线 index是输入数据的序号 center是激光雷达的中心
	for(int i=0;i<line_in_map.size();++i){
		if(myradar.linecrossseg(line_in_map[i])&&!myradar.parallel(line_in_map[i])){//相交但不重合
			Point temp = myradar.crosspoint(line_in_map[i]);
			double dist = center.distance(temp);
			put_into_array(index,dist,temp,center);
		}
	}
}
void get_min_with_circle(Line myradar,int index,Point center){
	for(int i=0;i<circle_in_map.size();++i){
		Point temp1,temp2;
		int num = circle_in_map[i].pointcrossline(myradar,temp1,temp2);
		if(num>=1){
			double dist = center.distance(temp1);
			put_into_array(index,dist,temp1,center);
		}
		if(num==2){
			double dist = center.distance(temp2);
			put_into_array(index,dist,temp2,center);
		}
	}	
}

int main(){
	double x = 10.0;//x y的坐标
	double y = 10.0;
	Point p(x,y);
	double AreaLen = 20;
	for(int i=0;i<1024;++i) min_distacne[i] = inf;
	Point leftdown(0,0);
	Point leftup(0,AreaLen);
	Point rightdown(AreaLen,0);
	Point rightup(AreaLen,AreaLen);
	// 四条场地的边界
	line_in_map.push_back(Line(leftdown,leftup));
	line_in_map.push_back(Line(leftdown,rightdown));
	line_in_map.push_back(Line(leftup,rightup));
	line_in_map.push_back(Line(rightdown,rightup));
	Line radar;//激光射线
	printf("The number of your angle\n");
	cin>>angle_num;
	printf("Please give %d lines and each line with one double\n",angle_num );
	for(int i=0;i<angle_num;++i){
		cin>>angleOfRadar[i];
	}
	printf("The number of lines\n");
	cin>>line_num;
	printf("Please give %d lines and each line with x1,y1,x2,y2\n",line_num );
	for(int i=0;i<line_num;++i){
		double x1,y1,x2,y2;
		cin>>x1>>y1>>x2>>y2;
		Line temp(Point(x1,y1),Point(x2,y2));
		line_in_map.pb(temp);
	}
	printf("The number of circles\n");
	cin>>circle_num;
	printf("Please give %d lines and each line with x,y,r\n",circle_num );
	for(int i=0;i<circle_num;++i){
		double circle_x,circle_y,circle_r;
		cin>>circle_x>>circle_y>>circle_r;
		circle temp(circle_x,circle_y,circle_r);
		circle_in_map.pb(temp);
	}
	for(int i = 0 ; i < angle_num ; ++i){
		double degree = angleOfRadar[i];
		if(sgn(degree-180.0)>0) degree -= 180;
		double angle = 1.0 * degree * pi / 180.0; //偏离x轴正方向的角度
		if(sgn(angleOfRadar[i]-180)==0){
			radar = Line(p,Point(p.x+1,p.y));
		}
		else radar = Line(p,angle);
		get_min_with_segment(radar,i,p);//与线段
		get_min_with_circle(radar,i,p);
	}
	for(int i = 0 ; i < angle_num ; ++i){
		printf("angle:%lf distance%lf\n",angleOfRadar[i],min_distacne[i] );
	}
	return 0;
}
