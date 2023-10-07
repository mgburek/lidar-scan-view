#ifndef SCAN_DATA_MODEL_H
#define SCAN_DATA_MODEL_H

#include <vector>

// ####   #####  #  #   #  #####  
// #   #  #   #  #  ##  #    #    
// ####   #   #  #  # # #    #    
// #      #   #  #  #  ##    #    
// #      #####  #  #   #    #   

struct Point
{
    Point(float x, float y, float z, float scale = 1.0) : _x{x}, _y{y}, _z{z}, _scale{scale} {};
    float x() const { return _x; }
    float y() const { return _y; }
    float z() const { return _z; }
    float scale() const { return _scale; }

private:
    float _x, _y, _z;
    float _scale = 1.0;
};

// #####  #####   ###   #   #     ####    ###   #####   ###   
// #      #      #   #  ##  #     #   #  #   #    #    #   #  
// #####  #      #####  # # #     #   #  #####    #    #####  
//     #  #      #   #  #  ##     #   #  #   #    #    #   #  
// #####  #####  #   #  #   #     ####   #   #    #    #   #  

class ScanData
{
    friend class ScanDataModel;

private:
    std::vector<Point> _points;
};

// #####  #####   ###   #   #     ####    ###   #####   ###      #   #  #####  ####   #####  #      
// #      #      #   #  ##  #     #   #  #   #    #    #   #     ## ##  #   #  #   #  #      #      
// #####  #      #####  # # #     #   #  #####    #    #####     # # #  #   #  #   #  #####  #      
//     #  #      #   #  #  ##     #   #  #   #    #    #   #     #   #  #   #  #   #  #      #      
// #####  #####  #   #  #   #     ####   #   #    #    #   #     #   #  #####  ####   #####  #####  

class ScanDataModel
{
public:
    ScanDataModel() : _scans(1, ScanData()){};

    void add(const Point &point);
    const std::vector<Point> &points();
    void removeAllScans();

private:
    std::vector<ScanData> _scans;
};

#endif // SCAN_DATA_MODEL_H