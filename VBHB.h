#pragma once 

#include <vector>
#include <memory>

#include "Vec.h"
#include "Mat.h"

class VBHB_NODE
{
private:
    std::vector<std::unique_ptr<float>> P;
    std::vector<std::unique_ptr<float>> bounding_box;
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
    std::unique_ptr<VBHB_NODE> node;
    std::unique_ptr<VBHB_NODE> right_node, left_node;

public:
    VBHB()
    {
        this->node = std::unique_ptr<VBHB_NODE>(new VBHB_NODE(true));
        this->right_node = nullptr;
        this->left_node = nullptr;
    }

    void insert(float triange[3], std::vector<std::unique_ptr<float>> P);
    
    static std::vector<std::unique_ptr<float>> 
    traverse(Vec<4, float> triange[3], std::unique_ptr<VBHB>& node)
    {
        std::vector<std::unique_ptr<float>> p;

        if (node->left_node == nullptr && node->right_node == nullptr) {
            
        }

        return p;
    }

    bool 
    SAT(Vec<4, float> triangle[3], std::vector<std::unique_ptr<float>>& polygon)
    {
        Vec<4, float> AB = triangle[0] - triangle[1], BC = triangle[1] - triangle[2], CA = triangle[2] - triangle[0];
        float AB_size_2 = dot(AB, AB), BC_size_2 = dot(BC, BC), CA_size_2 = dot(CA, CA);
        
        Vec<4, float> norm1 = AB - CA * (dot(AB, CA) / dot(CA, CA)), 
                      norm2 = BC - AB * (dot(BC, AB) / dot(AB, AB)),
                      norm3 = CA - BC * (dot(CA, BC) / dot(BC, BC));

        

        return 0;
    }
};

int orient(Vec<4, float> triangle[3]);