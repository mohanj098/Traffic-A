#include<iostream>
using namespace std;
double epsn=10e-9;
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
    int insert(int reg, float x, float v, float t){//help in calling the real insert
        root=in(root,reg,x-v*t,v);//now we'll insert every nodeavlx at time t=0
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
    int remove(float x, float t){
        root=de(root,x,t);
    }
};
int main(){}