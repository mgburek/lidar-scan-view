#include "scan_data_model.h"

// #####  #####   ###   #   #     ####    ###   #####   ###      #   #  #####  ####   #####  #      
// #      #      #   #  ##  #     #   #  #   #    #    #   #     ## ##  #   #  #   #  #      #      
// #####  #      #####  # # #     #   #  #####    #    #####     # # #  #   #  #   #  #####  #      
//     #  #      #   #  #  ##     #   #  #   #    #    #   #     #   #  #   #  #   #  #      #      
// #####  #####  #   #  #   #     ####   #   #    #    #   #     #   #  #####  ####   #####  #####  

void ScanDataModel::add(const Point &point)
{
    _scans[0]._points.push_back(point);
}

const std::vector<Point> &ScanDataModel::points()
{
    return _scans[0]._points;
}

void ScanDataModel::removeAllScans()
{
    for (ScanData &scan : _scans)
    {
        scan._points.clear();
    }
}