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
int InOut(const int&, const int&, const int&);
int Advance(int, int *, int, bool, std::vector<Vec<4, float>>&, const Vec<4, float>&);
bool SAT(const std::vector<Vec<4, float>>&, const std::vector<Vec<4, float>>&);


class VBHB_NODE
{
private:
    std::vector<Vec<4, float>> P;
    std::vector<Vec<4, float>> bounding_box;
    bool is_root;
    int64_t label;
    int64_t triangle_ind;
    int64_t color_ind;

public:
    VBHB_NODE(bool is_root_node, int64_t label_inp = 0, int64_t triangle_ind_inp = 0, int64_t color_ind_inp = 0) : 
        is_root(is_root_node), label(label_inp), triangle_ind(triangle_ind_inp), color_ind(color_ind_inp) {}
    ~VBHB_NODE()
    {
        P.clear();
        bounding_box.clear();
    }

    friend class VBHB;
};

class VBHB 
{

friend VBHB_NODE;

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
    
    static std::vector<std::vector<Vec<4, float>>> 
    traverse(std::vector<Vec<4, float>> triangle, VBHB* node)
    {
        std::vector<std::vector<Vec<4, float>>> res;

        if (node->left_node == nullptr && node->right_node == nullptr) {
            
            if (SAT(triangle, node->node->P)) {
                res.push_back(node->node->P);
            }

            return res;
        }

        if (!SAT(triangle, node->node->bounding_box)) {
            return res;
        }

        std::vector<std::vector<Vec<4, float>>> Left = traverse(triangle, node->left_node);
        std::vector<std::vector<Vec<4, float>>> Right = traverse(triangle, node->right_node);

        std::vector<std::vector<Vec<4, float>>> ans(Left.begin(), Left.end());
        ans.insert(Left.end(), Right.begin(), Right.end());

        return ans;
    }

    void 
    init_start_pos(const std::vector<Vec<4, float>>& points) 
    {
        std::copy(points.begin(), points.end(), std::back_inserter(this->node->P));
        std::copy(points.begin(), points.end(), std::back_inserter(this->node->bounding_box));
    }
};
