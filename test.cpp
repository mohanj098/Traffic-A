/* algo:-
I m gonna make two avl trees and one heap. one avl on the basis of registrarion no. other on the basis
of x-cordinate if two x-cordinate clash then i'll use registrtion no. to decide where to go left and 
right. heap is based on time of collision of two consecutive cars. each car will first overtake it's
consecutive car that is having x-cordinate greater than this car, so i'll store all the time of 
overtaking for all the cars in a heap and then we just have to call min-heap to find minimum time'
when we'll get then input to find first crossing then we'll go to heap and find minimium element
heap is augmented with registrattion no. so we get registration of those car then simply we'll printthem
we also have pointer to heap to every node augmented in the avl based on registration no. to update 
the values if some car come inbetween two cars  i have assumed cars are going from left to right.*/

#include<iostream>
using namespace std;
double epsn=10e-9;
float curr=0;//same for all and easily accessible
float minimum=1000000000;//minimum time assumption 

template <class t1, class t2>//using templates
class car{//node for the first avl that will be on the basis of x-cordinate
    private:
        t1 reg;
        t2 x;
        t2 v;
        t2 t;//assumed x, v, t to be of same type otherwise i have to use more templates
        int height;
        car<t1,t2>* left;
        car<t1,t2>* right;
        /*heapnode* pointer;//pointer to heap to the key corresponding to the time it'll take 
                            // to cross it's successor*/  
    public: 
        template<class u1, class u2> friend class avl; //to access private members
};
template <class t1, class t2>
class avl{
    //wherever i'll create car object i'll use car<t1,t2>
private:
    car<t1, t2>* root;//we'll get these t2, t1 from main

public:
    avl(){//constructor
        root=NULL;
    }
    void getxandv1(car<t1,t2>* node, t1 reg,t2* x, t2* v){//actual to give x-cordinate and v;
        if(node->reg==reg){
            *x=node->x;
            *v=node->v;
            return;
        }
        else if(reg>node->reg){
            getxandv1(node->right,reg,x,v);
            return;
        }
        else{
            getxandv1(node->left,reg,x,v);
            return;
        }
    }
    void getxandv(t1 reg, t2* x, t2* v){//to get x and v this function will help in calling of the above function
        getxandv1(root, reg, x, v);
    }
    int se(car<t1,t2> *node, t1 reg){ //to search that avl which consists cars on basis of reg
        if(node==NULL){
            return 0;//not found return 0;
        }
        else if(node->reg==reg){
            return 1; //if found return 1
        }
        else if(reg>node->reg){
            return se(node->right, reg);
        }
           else{
            return se(node->left, reg);
        }
    }
    int search(t1 reg){
        return se(root, reg);
    }
    int getheight(car<t1,t2>* a){//it'll return height;
        if(a==NULL){
            return 0;
        }
        else{
            return a->height;
        }
    }
    int max(int a, int b){
        if(a>b){
            return a;
        }
        else{
            return b;
        }
    }
    car<t1,t2>* leftrotation(car<t1,t2>* fault){ //  left rotation to make avl
        car<t1,t2>* temp1=fault->right;
        car<t1,t2>* temp2=temp1->left;
        fault->right=temp2;
        temp1->left=fault;
        fault->height = 1+max(getheight(fault->left), getheight(fault->right));
        temp1->height = 1+max(getheight(temp1->left), getheight(temp1->right));
        return temp1;
    }
    car<t1,t2>* rightrotation(car<t1,t2>* fault){//right rotation
        car<t1,t2>* temp1 = fault->left;
        car<t1,t2>* temp2 = temp1->right;
        fault->left=temp2;
        temp1->right=fault;
        fault->height = 1+max(getheight(fault->left), getheight(fault->right));
        temp1->height = 1+max(getheight(temp1->left), getheight(temp1->right));
        return temp1;
    }
    car<t1,t2>* balance(car<t1,t2>* fault, int reg){//for checking and calling for rotation function
        int bf = getheight(fault->left) - getheight(fault->right);//balance factor
        if(bf > 1 && fault->left!=NULL && reg < fault->left->reg){//condition to check
        return rightrotation(fault);
        }
        else if(bf >1 && fault->left!=NULL && reg > fault->left->reg){
            fault->left=leftrotation(fault->left);
            return rightrotation(fault);
        }
        else if(bf < -1 && fault->right!=NULL && reg>fault->right->reg){
            return leftrotation(fault);
        }
        else if(bf < -1 && fault->right!=NULL && reg<fault->right->reg){
            fault->right=leftrotation(fault->right);
            return leftrotation(fault);
        }
        else{// if not unbalanced return it as it is
            return fault;
        }
    }
    car<t1,t2>* in(car<t1,t2>* car1, t1 reg, t2 x, t2 v){//actual function to insert
        if(car1==NULL){
            car<t1,t2>* temp = new car<t1,t2>;
            temp->reg = reg;
            temp->x = x;//storing at t=0;
            temp->t = curr;//inserting at t=curr actually
            temp->v = v;
            temp->height=1;//height=1 of a leaf
            temp->left = NULL;
            temp->right = NULL;
            car1=temp;
            return car1;
        }
        else if(reg>car1->reg){
	    	car1->right=in(car1->right,reg,x,v);
            car1->height=1+max(getheight(car1->left), getheight(car1->right));//update
        }
        else{
            car1->left=in(car1->left,reg,x,v);
            car1->height=1+max(getheight(car1->left), getheight(car1->right));//update
        }
        car1 = balance(car1, reg); // will check and then balance the tree
        return car1;
    }   
    int insert(t1 reg, t2 x, t2 v){//help in calling the 'in' function to insert from main
        if(se(root,reg)==0){ //checking that the element exist or not;
            root=in(root,reg,x-v*curr,v);//inserting at t=0 in the tree
            return 1;
        }
        else{
            return 0;
        }        
    }
    void copy(car<t1,t2>* a, car<t1,t2>*b){//to copy content of a node to other
        if(b==NULL){
            a=NULL;
            return;
        }
        else{
            a->reg=b->reg;
            a->x=b->x;
            a->t=b->t;
            a->v=b->v;
            a->height=b->height;
            a->left=b->left;
            a->right=b->right;
        }
    }
    car<t1,t2>* de(car<t1,t2>* car1, t1 reg){ //actual deleting function
        if(car1->reg==reg){//found now we have to consider three cases
            if(car1->left==NULL && car1->right==NULL){
                free(car1);
                car1=NULL;
                return car1;
            }
            else if(car1->left!=NULL && car1->right==NULL){
                car<t1,t2>* temp=car1->left;
                copy(car1, car1->left);
                free(temp);
                temp=NULL;
                return car1;
            }
            else{
                car<t1,t2>* temp;
                temp=car1->right;
                while(temp->left!=NULL){
                    temp=temp->left;
                }
                car1->reg=temp->reg;
                car1->x=temp->x;
                car1->t=temp->t;
                car1->v=temp->v;
                car1->right= de(car1->right, temp->reg);
                car1->height=1+max(getheight(car1->left), getheight(car1->right));//height update
                return car1;

            }
        }
        else if(reg > car1->reg){
            car1->right=de(car1->right,reg);
            car1->height=1+max(getheight(car1->left), getheight(car1->right));//height update
        }
        else{
            car1->left=de(car1->left,reg);
            car1->height=1+max(getheight(car1->left), getheight(car1->right));
        }
        int bf = getheight(car1->left) - getheight(car1->right);//rotation 
        if(bf>1 && getheight(car1->left->left) - getheight(car1->left->right) >= 0){
            return rightrotation(car1);
        }
        else if(bf>1 && getheight(car1->left->left) - getheight(car1->left->right) < 0){
            car1->left=leftrotation(car1->left);
            return rightrotation(car1);
        }
        else if(bf<-1 && getheight(car1->right->left) - getheight(car1->right->right) > 0){
            car1->right=rightrotation(car1->right);
            return leftrotation(car1);
        }
        else if(bf<-1 && getheight(car1->right->left) - getheight(car1->right->right) <=0){
            return leftrotation(car1);
        }
        else{
            return car1;
        }

    }
    int remove(t1 reg){
        if(se(root,reg)==1){//check of existence
            root=de(root,reg);
            return 1;//sucessfull
        }
        else{
            return 0;//unsucessfull
        }
    }   
};

class nodeavlx{//node for avl tree that is on the basis of x cordinate and if there comes a 
                //situation of collision then we'll compare them on the basis of registration;
    private:
    int regself;//slef registrarion no.
    float xself;//self x cordinate
    float vself;//self velocity
    int regsucc;
    nodeavlx* left;
    nodeavlx* right;
    int height;// to find balnce factor;
    float time;//time at which it'll cross it's

    public:
    friend class avlx;//now avl tree can access its nodes

};

class avlx{//avl tree based on x-cordinate and if two x-cordinates collide we'll use registration
            //number
    private:
    nodeavlx* root;//pointer to node;
    public:
    avlx(){//constructor
        root=NULL;
    }
    int getheight(nodeavlx* a){
        if(a==NULL){
            return 0;
        }
        else{
            return a->height;
        }
    }
    float mod(float x){
        if(x>=0){
            return x;
        }
        else{
            return -1*x;
        }
    }
    int max(int a, int b){
        if(a>b){
            return a;
        }
        else{
            return b;
        }
    }
    nodeavlx* successor(nodeavlx* node, nodeavlx* required){//to find successor 
        if(required->right!=NULL){//if not then minimum in right tree;
            nodeavlx* temp=required->right;
            while(temp->left!=NULL){
                temp=temp->left;
            }
            return temp;
        }
        nodeavlx* temp=node;
        while(node!=NULL){
            if(node->xself>required->xself){
                temp=node;
                node=node->left;
            }
            else if(node->xself<required->xself){
                node=node->right;//successor won't change 
            }
            else{//if x clash;//succesor won't change at this point
                if(node->regsucc>required->regsucc){
                    node=node->left;
                }
                else if(node->regsucc<required->regsucc){
                    node=node->right;
                }
                else{
                    return temp;
                }
            }
        }
        return temp;
    }
    nodeavlx* leftrotation(nodeavlx* fault){
        nodeavlx* temp1=fault->right;
        nodeavlx* temp2=temp1->left;
        fault->right=temp2;
        temp1->left=fault;
        fault->height = 1+max(getheight(fault->left), getheight(fault->right));
        temp1->height = 1+max(getheight(temp1->left), getheight(temp1->right));
        return temp1;
    }
    nodeavlx* rightrotation(nodeavlx* fault){
        nodeavlx* temp1 = fault->left;
        nodeavlx* temp2 = temp1->right;
        fault->left=temp2;
        temp1->right=fault;
        fault->height = 1+max(getheight(fault->left), getheight(fault->right));
        temp1->height = 1+max(getheight(temp1->left), getheight(temp1->right));
        return temp1;
    }
    nodeavlx* balance(nodeavlx* fault, float x, float v){
        int bf = getheight(fault->left) - getheight(fault->right);
        /*cout<<bf<<"for"<<fault->reg<<endl;*/
        if(bf > 1 && fault->left!=NULL && x < (fault->left->xself)){
            return rightrotation(fault);
        }
        else if(bf >1 && fault->left!=NULL && x > (fault->left->xself)){
            fault->left=leftrotation(fault->left);
            return rightrotation(fault);
        }
        else if(bf < -1 && fault->right!=NULL && x > (fault->right->xself)){
            return leftrotation(fault);
        }
        else if(bf < -1 && fault->right!=NULL && x <  (fault->right->xself)){
            fault->right=leftrotation(fault->right);
            return leftrotation(fault);
        }
        else{
            return fault;
        }
    }
    nodeavlx* in(nodeavlx* nodeavlx1, int reg, float x, float v){
        if(nodeavlx1==NULL){
            nodeavlx* temp = new nodeavlx;
            temp->regself = reg;
            temp->xself = x;
            temp->vself = v;
            temp->height=1;
            temp->left = NULL;
            temp->right = NULL;
            if(root==NULL){//if root=NULL no successor
                temp->time=-1;
                nodeavlx1=temp;
                return nodeavlx1;
            }
            nodeavlx* temp2=successor(root,temp);//finding successor
            temp->regsucc=temp2->regself;//storing successor
            temp->time=(temp2->xself-temp->xself)/(temp->vself-temp2->vself);//time to cross can be negative too
            nodeavlx1=temp;
            return nodeavlx1;
        }
        else if(mod(x-nodeavlx1->xself)<=epsn){
            if(nodeavlx1->regself<reg){
                nodeavlx1->right=in(nodeavlx1->right,reg,x,v);
            }
            else{
                nodeavlx1->left=in(nodeavlx1->left,reg,x,v);
            }
        }
        else if(x> nodeavlx1->xself){
			nodeavlx1->right=in(nodeavlx1->right,reg,x,v);
            nodeavlx* temp=successor(root,nodeavlx1);//for updating time
            nodeavlx1->time=(temp->xself-nodeavlx1->xself)/(nodeavlx1->vself-temp->xself);//update
            nodeavlx1->height=1+max(getheight(nodeavlx1->left), getheight(nodeavlx1->right));
        }
        else{
            nodeavlx1->left=in(nodeavlx1->left,reg,x,v);
            nodeavlx* temp=successor(root,nodeavlx1);//updating time
            nodeavlx1->time=(temp->xself-nodeavlx1->xself)/(nodeavlx1->vself-temp->xself);//update
            nodeavlx1->height=1+max(getheight(nodeavlx1->left), getheight(nodeavlx1->right));
        }
        nodeavlx1 = balance(nodeavlx1, x,v);//to get avl;
    }
    int insert(int reg, float x, float v){//help in calling the real insert
        root=in(root,reg,x-v*curr,v);//now we'll insert every nodeavlx at time t=0
        return 1;//after insertion;
    }
    void copy(nodeavlx* a, nodeavlx*b){
        if(b==NULL){
            a=NULL;
            return;
        }
        else{
            a->regself=b->regself;
            a->xself=b->xself;
            a->vself=b->vself;
            a->time=b->time;
            a->height=b->height;
            a->regsucc=b->regsucc;
            a->left=b->left;
            a->right=b->right;
        }
    }
    nodeavlx* de(nodeavlx* nodeavlx1, int x, float v){
        if((nodeavlx1->regself-x)<=epsn){
            if(nodeavlx1->left==NULL && nodeavlx1->right==NULL){
                free(nodeavlx1);
                nodeavlx1=NULL;
                return nodeavlx1;
            }
            else if(nodeavlx1->left!=NULL && nodeavlx1->right==NULL){
                nodeavlx* temp=nodeavlx1->left;
                copy(nodeavlx1, nodeavlx1->left);
                free(temp);
                temp=NULL;
                return nodeavlx1;
            }
            else{
                nodeavlx* temp;
                temp=nodeavlx1->right;
                while(temp->left!=NULL){
                    temp=temp->left;
                }
                nodeavlx* temp2=successor(root,nodeavlx1);
                nodeavlx1->regself=temp->regself;
                nodeavlx1->xself=temp->xself;
                nodeavlx1->vself=temp->vself;
                nodeavlx1->time=(temp2->xself-nodeavlx1->xself)/(nodeavlx1->vself-temp2->vself);
                //time updation
                nodeavlx1->right= de(nodeavlx1->right, temp->xself, temp->vself);
                return nodeavlx1;

            }
        }
        else if(x > nodeavlx1->xself){
            nodeavlx1->right=de(nodeavlx1->right,x,v);
            nodeavlx* temp=successor(root,nodeavlx1);//for updating time
            nodeavlx1->time=(temp->xself-nodeavlx1->xself)/(nodeavlx1->vself-temp->xself);//update
            nodeavlx1->height=1+max(getheight(nodeavlx1->left), getheight(nodeavlx1->right));
        }
        else{
            nodeavlx1->left=de(nodeavlx1->left,x,v);
            nodeavlx* temp=successor(root,nodeavlx1);//for updating time
            nodeavlx1->time=(temp->xself-nodeavlx1->xself)/(nodeavlx1->vself-temp->xself);//update
            nodeavlx1->height=1+max(getheight(nodeavlx1->left), getheight(nodeavlx1->right));
        }
        //now just we have to balance
        int bf = getheight(nodeavlx1->left) - getheight(nodeavlx1->right);
        if(bf>1 && getheight(nodeavlx1->left->left) - getheight(nodeavlx1->left->right) >= 0){
            return rightrotation(nodeavlx1);
        }
        else if(bf>1 && getheight(nodeavlx1->left->left) - getheight(nodeavlx1->left->right) < 0){
            nodeavlx1->left=leftrotation(nodeavlx1->left);
            return rightrotation(nodeavlx1);
        }
        else if(bf<-1 && getheight(nodeavlx1->right->left) - getheight(nodeavlx1->right->right) > 0){
            nodeavlx1->right=rightrotation(nodeavlx1->right);
            return leftrotation(nodeavlx1);
        }
        else if(bf<-1 && getheight(nodeavlx1->right->left) - getheight(nodeavlx1->right->right) <=0){
            return leftrotation(nodeavlx1);
        }
        else{
            return nodeavlx1;
        }
    }
    int remove(float x, float v){
        root=de(root,x-v*curr,v);
    }
    void mintime1(nodeavlx* node){//this will give us the time but in O(n);
        if(node==NULL){
            return;
        }
        if(minimum>node->time){
            minimum=node->time;
        }
        mintime1(node->left);
        mintime1(node->right);
    }
    void mintime(){
        mintime1(root);
    }
    void printmin(nodeavlx* node){
        if(node==NULL){
            return;
        }
        if(node->time==minimum){
            cout<<node->regself<<node->regsucc<<endl;
        }
        printmin(node->left);
        printmin(node->right);
    }
};

class highway{//it'll consist all the the object of three class will help in reaching everywhere
    private:
    avl<int,float> avl1;
    avlx avl2;

    public:
    int insert(int reg, float x, float v){
        int n=avl1.insert(reg,x,v);
        if(n==1){//insert was successful so we can now insert on other trees also
            avl2.insert(reg,x,v);
            return 1;//insert was successful;
        }
        else{
            return 0;//unsuccessful
        }
    }
    int remove(int reg){
        int n=avl1.search(reg);
        if(n==1){// so value exist we can proceed next
            float x,v;//we'll get these from avl1 that is on the basis of reg;
            avl1.getxandv(reg,&x,&v);
            avl1.remove(reg);
            avl2.remove(x,v);
            return 1;
        }
        else{
            return 0;
        }
    }
    void mintime(){
        avl2.mintime();
    }
};


int main(){
    highway road;
    int m;
    cin>>m;
    while(m--){
        int n;
        cin>>n;
        if(n==1){
            int reg;
            float x,v;
            cin>>reg>>x>>v;
            road.insert(reg,x,v);
        }
        else if(n==2){
            int reg;
            cin>>reg;
            road.remove(reg);
        }
        else if(n==3){
            road.mintime();
        }
    }
}