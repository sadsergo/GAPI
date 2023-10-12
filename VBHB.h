#pragma once 

#include <vector>
#include <memory>

#include "Vec.h"
#include "Mat.h"

enum 
{
    Qin = 2,
    Pin = 1,
    Unknown = 0,
};

std::vector<Vec<4, float>> intersect(const std::vector<Vec<4, float>>&, const std::vector<Vec<4, float>>&);
int orient(Vec<4, float> triangle[3]);
int areaSign(const Vec<4, float>&, const Vec<4, float>&, const Vec<4, float>&);
char SegSegInt(const Vec<4, float>&, const Vec<4, float>&, const Vec<4, float>&, Vec<4, float>&);
char ParallelInt(const Vec<4, float>&, const Vec<4, float>&, const Vec<4, float>&, const Vec<4, float>&, Vec<4, float>&);
void Assigndi(Vec<4, float>&, const Vec<4, float>&);
bool Between(const Vec<4, float>&, const Vec<4, float>&, const Vec<4, float>&);
bool Collinear(const Vec<4, float>&, const Vec<4, float>&, const Vec<4, float>&);
int InOut(const Vec<4, float>&, const int&, const int&, const int&);
int Advance(int, int *, int, bool, const Vec<4, float>&);


class VBHB_NODE
{
private:
    std::vector<Vec<4, float>> P;
    std::vector<Vec<4, float>> bounding_box;
    bool is_root;

public:
    VBHB_NODE(bool is_root_node) : is_root(is_root_node) {}
    ~VBHB_NODE()
    {
        P.clear();
        bounding_box.clear();
    }

    friend class VBHB;
};

class VBHB 
{
private:
    VBHB_NODE* node;
    VBHB *right_node, *left_node;

public:
    VBHB()
    {
        // this->node = std::unique_ptr<VBHB_NODE>(new VBHB_NODE(true));
        this->node = new VBHB_NODE (true);
        this->right_node = nullptr;
        this->left_node = nullptr;
    }

    void insert(float triange[3], std::vector<Vec<4, float>>& P);
    
    static std::vector<Vec<4, float>> 
    traverse(std::vector<Vec<4, float>> triangle, VBHB* node)
    {
        std::vector<Vec<4, float>> p;
        
        if (node->left_node == nullptr && node->right_node == nullptr) {
            if (intersect(triangle, node->node->P).size() == 0) {
                return node->node->P;
            }
            else {
                return p;
            }
        }

        if (intersect(triangle, node->node->bounding_box).size() == 0) {
            return p;
        }

        std::vector<Vec<4, float>> Left = traverse(triangle, node->left_node);
        std::vector<Vec<4, float>> Right = traverse(triangle, node->right_node);

        std::vector<Vec<4, float>> ans(Left.begin(), Left.end());
        ans.insert(Left.end(), Right.begin(), Right.end());

        return ans;
    }

    bool 
    SAT(Vec<4, float> triangle[3], std::vector<Vec<4, float>>& polygon)
    {
        Vec<4, float> AB = triangle[0] - triangle[1], BC = triangle[1] - triangle[2], CA = triangle[2] - triangle[0];
        float AB_size_2 = dot(AB, AB), BC_size_2 = dot(BC, BC), CA_size_2 = dot(CA, CA);
        
        Vec<4, float> norm1 = AB - CA * (dot(AB, CA) / dot(CA, CA)), 
                      norm2 = BC - AB * (dot(BC, AB) / dot(AB, AB)),
                      norm3 = CA - BC * (dot(CA, BC) / dot(BC, BC));

        

        return 0;
    }
};
