#include "bitmap_image.hpp"
#include "custom.hpp"
#include <bits/stdc++.h>
#include <stdlib.h>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <sstream>

using namespace std;

point point_init(string s)
{
    stringstream ss(s);
    double word;
    vector<double> values;
    while (ss >> word) {
        values.push_back(word);
    }
   point temp(values[0],values[1],values[2]);
   return temp;
}

struct Vector{
    double x,y,z;
    Vector(){};
    Vector(double a,double b, double c){
        x=a;
        y=b;
        z=c;
    }
    Vector operator+ (Vector p) {
	    Vector res;
	    res.x = x + p.x;
	    res.y = y + p.y;
	    res.z = z + p.z;
	    return res; }
Vector operator- (Vector p) {
	    Vector res;
	    res.x = x - p.x;
	    res.y = y - p.y;
	    res.z = z - p.z;
	    return res; }
    Vector operator*(double k) {
	    Vector res;
	    res.x = x * k;
	    res.y = y * k;
	    res.z = z * k;
	    return res; }
	    void normalize(){
            double value = sqrt(x*x + y*y + z*z);
            x /=  value;
            y /=  value;
            z /=  value;
}
	    void print(){
	    cout<<"Vector printing..."<<endl;
	    cout<<"x: "<<x<<" y: "<<y<<" z: "<<z<<endl;
	    }
	    void scrutinize(){
        if(x == -0){
            x+=1;
            x-=1;
	    }
	    if(y == -0){
            y+=1;
            y-=1;
	    }
	    if(z == -0){
            z+=1;
            z-=1;
	    }
	    }
};
double dotProduct(Vector vect_A, Vector vect_B)
{
    double product = 0;
    product += vect_A.x * vect_B.x;
    product += vect_A.y * vect_B.y;
    product += vect_A.z * vect_B.z;
    product+=1;
    product -=1;
    return product;
}

Vector crossProduct(Vector vect_A, Vector vect_B)
  {
     Vector cross_P;
     cross_P.x = vect_A.y * vect_B.z - vect_A.z * vect_B.y;
     cross_P.y = vect_A.z * vect_B.x - vect_A.x * vect_B.z;
     cross_P.z = vect_A.x * vect_B.y - vect_A.y * vect_B.x;
     cross_P.scrutinize();
     return cross_P;
}

Vector vector_init(string s)
{
    stringstream ss(s);
    double word;
    vector<double> values;
    while (ss >> word) {
        values.push_back(word);
    }
   Vector temp(values[0],values[1],values[2]);
   return temp;
}

Vector Rod(Vector x,Vector a,double angle){
    Vector res;
    double cosValue,sinValue;
    cosValue = cos(angle * PI /180.0);
    sinValue = sin(angle * PI /180.0);
    cout << fixed << setprecision(6) << cosValue<<" "<<sinValue <<endl;
    res = x * cosValue + a * ((1 - cosValue)*dotProduct(a,x))  ;
    res = res +  crossProduct(a,x) * sinValue;
    return res;
}
Vector eye,look,up;
double fovY,aspectRatio,near_,far_;
int main()
{
matrix current_trans(4,4,"identity");
//current_trans.print();
stack<matrix> trans_stack;
stack<matrix> ins_history;
vector<triangle> triangles;
bool push_flag = false;
string myText;
ifstream MyReadFile("scene.txt");
int lineCount=1,i,j,k=1;
ins_history.push(current_trans);
while (getline (MyReadFile, myText)) {

    switch(lineCount){
    case 1:
        eye = vector_init(myText);
        //eye.print();
        break;
    case 2:
        look = vector_init(myText);
       // look.print();
        break;
    case 3:
        up = vector_init(myText);
        //up.print();
        break;
    case 4:
        stringstream ss(myText);
        double word;
        vector<double> values;
        while (ss >> word) {
            values.push_back(word);
        }
        fovY = values[0];
        aspectRatio = values[1];
        near_ = values[2];
        far_ = values[3];
        break;
    }
    if(lineCount > 4){
        if(myText == "triangle"){
            vector<point> temp;
            for(int i=0;i<3;i++){
            getline (MyReadFile, myText);
                temp.push_back(point_init(myText));
            }
            triangle a(temp[0],temp[1],temp[2]);
            //cout<<"Current Trans Matrix"<<endl;
            //current_trans.print();
            a.a = current_trans * a.a;
            a.b = current_trans * a.b;
            a.c =  current_trans * a.c;
            a.precision();
            triangles.push_back(a);
            //cout<<"Triangle: "<<k<<endl;
            k++;
            //a.print();
        }
    else if(myText == "scale"){
        cout<<"scale\n";
        getline (MyReadFile, myText);
        matrix t(4,4);
        stringstream ss(myText);
        double sValue;
        i=0,j=0;
        while(ss >> sValue){
            t.mat[i*4+j ] = sValue;
            i++;
            j++;
        }
        t.mat[i*4+j]=1;
        //t.print();
        if(current_trans.is_identity_matrix()){
            current_trans = t;
            //cout<<"init Current trans\n";
            //current_trans.print();
        }
        else{
            current_trans = current_trans * t;
             //cout<<"init Current trans\n";
            //current_trans.print();
        }
        ins_history.push(current_trans);
        if(push_flag){
            trans_stack.push(current_trans);
            //cout<<"top\n";
            //trans_stack.top().print();
            push_flag = false;
        }
    }
        else if(myText == "translate"){
        cout<<"translate\n";
        getline (MyReadFile, myText);
        matrix t(4,4,"identity");
        stringstream ss(myText);
        double tValue;
        i=0;
        while(ss >> tValue){
            t.mat[i*4+3]=tValue;
            i++;
        }
        //t.print();
        if(current_trans.is_identity_matrix()){
            current_trans = t;
             //cout<<"init Current trans\n";
             //current_trans.print();
        }
        else{
            current_trans = current_trans * t;
             //cout<<" Current trans\n";
             //current_trans.print();
        }
        ins_history.push(current_trans);
        //cout<<"top history\n";
        //ins_history.top().print();
        if(push_flag){
            trans_stack.push(current_trans);
            //cout<<"top\n";
            //trans_stack.top().print();
            push_flag = false;
        }
    }
    else if(myText == "rotate"){
        cout<<"Rotate"<<endl;
        getline (MyReadFile, myText);
        matrix t(4,4,"identity");
        stringstream ss(myText);
        double angle;
        Vector a,c1,c2,c3;
        ss >> angle;
        ss >> a.x;
        ss >> a.y;
        ss >> a.z;
        a.normalize();
        c1 = Rod(Vector(1,0,0),a,angle);
        c2 = Rod(Vector(0,1,0),a,angle);
        c3 = Rod(Vector(0,0,1),a,angle);
    t.mat[0] = c1.x;
    t.mat[1] = c2.x;
    t.mat[2] = c3.x;
    t.mat[3] = 0;
    t.mat[4] = c1.y;
    t.mat[5] = c2.y;
    t.mat[6] = c3.y;
    t.mat[7] = 0;
    t.mat[8] = c1.z;
    t.mat[9] = c2.z;
    t.mat[10] = c3.z;
    t.mat[11] = 0;
    //cout<<"Rotation Matrix\n";
    //t.print();
    //cout<<endl;
     if(current_trans.is_identity_matrix()){
            current_trans = t;
             //cout<<"init Current trans\n";
             //current_trans.print();
        }
        else{
            current_trans = current_trans * t;
             //cout<<" Current trans\n";
             //current_trans.print();
        }
        ins_history.push(current_trans);
        //cout<<"top history\n";
        //ins_history.top().print();
        if(push_flag){
            trans_stack.push(current_trans);
            //cout<<"top\n";
            //trans_stack.top().print();
            push_flag = false;
        }
    }
    else if(myText == "push"){
        cout<<"push\n";
        push_flag = true;
    }
    else if(myText == "pop"){
        cout<<"pop\n";
        while(!(ins_history.top() == trans_stack.top())){
            ins_history.pop();
        }
        //cout<<"check\n";
       // trans_stack.top().print();
        ins_history.pop();
        trans_stack.pop();
        current_trans = ins_history.top();
        //cout<<"After pop\n";
        //current_trans.print();
    }
    else if(myText == "end"){
        break;
    }

    }
    lineCount++;
}

MyReadFile.close();
string fName = "stage1.txt";
ofstream MyFile(fName);

for(i=0;i<triangles.size();i++){
    triangle t = triangles.at(i);
    //triangles.pop_back();
    MyFile<<t<<endl;
}
MyFile.close();
//View Transformation

Vector l,r,u;
l = look - eye;
l.normalize();
r = crossProduct(l,up);
r.normalize();
u = crossProduct(r,l);

matrix T_matrix(4,4,"identity");
T_matrix.mat[3]= -eye.x;
T_matrix.mat[7]= -eye.y;
T_matrix.mat[11]= -eye.z;
matrix R_matrix(4,4,"identity");
R_matrix.mat[0] = r.x;
R_matrix.mat[1] = r.y;
R_matrix.mat[2] = r.z;
R_matrix.mat[4] = u.x;
R_matrix.mat[5] = u.y;
R_matrix.mat[6] = u.z;
R_matrix.mat[8] = -l.x;
R_matrix.mat[9] = -l.y;
R_matrix.mat[10] = -l.z;
matrix V_matrix = R_matrix * T_matrix;
MyFile.open("stage2.txt");
for(i=0;i<triangles.size();i++){
    triangle t = triangles.at(i);
    //t.print();
    t = V_matrix * t;
    //t.print();
    triangles.insert(triangles.begin()+i,t);
    triangles.erase(triangles.begin()+i+1);
    MyFile<<t<<endl;
}
MyFile.close();

//Projection Transformation
double fovX,t_value,r_value;
fovX = fovY * aspectRatio;
//cout<<fovX<<endl;
t_value = near_ * tan(fovY*PI/360.0);
r_value = near_ * tan(fovX*PI/360.0);
//cout<<near_<<" "<<far_<<endl;
//cout<<t_value<<" "<<r_value<<endl;
matrix P_matrix(4,4);
P_matrix.mat[0] = near_/r_value;
P_matrix.mat[5] =near_/t_value;
P_matrix.mat[10] = - (far_ + near_) /(far_ - near_);
P_matrix.mat[11] =- (2 * far_ * near_)/(far_- near_);
P_matrix.mat[14] = -1;
MyFile.open("stage3.txt");
for(i=0;i<triangles.size();i++){
    triangle t = triangles.at(i);
    t = P_matrix * t;
    t.set_max_values();
    //t.print();
    //cout<<t.boundary_values[0]<<" "<<t.boundary_values[1]<<endl;
    //cout<<t.boundary_values[2]<<" "<<t.boundary_values[3]<<endl;
    triangles.insert(triangles.begin()+i,t);
    triangles.erase(triangles.begin()+i+1);
    MyFile<<t<<endl;
}
MyFile.close();
double screen_width,scree_height,left_limit_x,bottom_limit_y,z_near,z_far;
MyReadFile.open("config.txt");
for(lineCount=1;lineCount<5;lineCount++){
        getline(MyReadFile,myText);
        stringstream ss(myText);
switch(lineCount){
    case 1:
        ss>>screen_width;
        ss>>scree_height;
        break;
    case 2:
        ss>>left_limit_x;
        break;
    case 3:
        ss>>bottom_limit_y;
        break;
    case 4:
        ss>>z_near;
        ss>>z_far;
        break;
    }
}
MyReadFile.close();
cout<<screen_width<<" "<<scree_height<<" "<<left_limit_x<<" "<<bottom_limit_y<<" "<<z_near<<" "<<z_far<<endl;
Z_buffer z_buffer(screen_width,scree_height,z_far);
//z_buffer.print();

double dx,dy,top_y,left_x,bottom_y,right_x;
dx = -(2*left_limit_x)/screen_width;
dy = -(2*bottom_limit_y)/scree_height;
top_y = -bottom_limit_y-dy/2;
bottom_y = bottom_limit_y + dy/2;
left_x = left_limit_x + dx/2;
right_x = -left_limit_x - dx/2;
triangle temp(point(-1,10,0),point(-10,1,0),point(-2,-2,0));
bitmap_image image(screen_width,scree_height);
for(i=0;i<screen_width;i++){
    for(j=0;j<500;j++)
        image.set_pixel(i,j,0,0,0);
}
// Z-Buffer
double scan_ty,scan_by,scan_lx,scan_rx,scan_zu,scan_zd;
point a,b;
for(i=0;i<triangles.size();i++){
    triangle t = triangles.at(i);
   if(t.boundary_values[2] < bottom_y)
        scan_by = bottom_y;
    else
        scan_by = t.boundary_values[2];

     if(t.boundary_values[3] > top_y)
        scan_ty = top_y;
    else
        scan_ty = t.boundary_values[3];
    double temp,temp2,dz;
    while(scan_ty >= scan_by){
            scan_lx = 0;
            scan_rx = 0;
            scan_zu = 0;
            scan_zd = 0;
        if((t.b.y - t.a.y) !=0){
            temp = t.a.x + (scan_ty - t.a.y) * (t.b.x -t.a.x) / (t.b.y - t.a.y);
            temp2 = t.a.z + (scan_ty - t.a.y) * (t.b.z -t.a.z) / (t.b.y - t.a.y);
            if(temp >= t.boundary_values[0] && temp <= t.boundary_values[1] ){
                if(temp < scan_lx)
                {
                    scan_lx = temp;
                    scan_zd = temp2;
                }
                else{
                    scan_rx = temp;
                    scan_zu = temp2;
                }
            }
        }
        if((t.b.y - t.c.y) !=0){
            temp = t.c.x + (scan_ty - t.c.y) * (t.b.x -t.c.x) / (t.b.y - t.c.y);
            temp2 = t.c.z + (scan_ty - t.c.y) * (t.b.z -t.c.z) / (t.b.y - t.c.y);
            if(temp >= t.boundary_values[0] && temp <= t.boundary_values[1] ){
                if(temp < scan_lx)
                {
                    scan_lx = temp;
                    scan_zd = temp2;
                }
                else{
                    scan_rx = temp;
                    scan_zu = temp2;
                }
        }}
        if((t.c.y - t.a.y) !=0){
            temp = t.a.x + (scan_ty - t.a.y) * (t.c.x -t.a.x) / (t.c.y - t.a.y);
            temp2 = t.a.z + (scan_ty - t.a.y) * (t.c.z -t.a.z) / (t.c.y - t.a.y);
            if(temp >= t.boundary_values[0] && temp <= t.boundary_values[1] ){
                if(temp < scan_lx)
                {
                    scan_lx = temp;
                    scan_zd = temp2;
                }
                else{
                    scan_rx = temp;
                    scan_zu = temp2;
                }
        }}
        dz = (scan_zu - scan_zd)/((scan_rx - scan_lx)/dx);
        k = s
        while(scan_lx <=scan_rx){
            if()
            scan_lx+=dx;
        }
        scan_ty -= dy;
    }
}
image.save_image("output.bmp");;

return 0;
}

