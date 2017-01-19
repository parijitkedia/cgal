#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/property_map.h>
#include <CGAL/IO/read_las_points.h>

#include <utility>
#include <vector>
#include <fstream>

// types
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT FT;
typedef Kernel::Point_3 Point;

int main(int argc, char*argv[])
{
  const char* fname = (argc>1) ? argv[1] : "data/pig_points.las";
  // Reads a .ply point set file with normal vectors and colors

  std::vector<Point> points; // store points
  std::ifstream in(fname);

  if (!in ||
      !CGAL::read_las_points_with_properties
      (in,
       std::back_inserter (points),
       CGAL::cpp11::make_tuple
       (CGAL::Identity_property_map<Point>(),
        Kernel::Construct_point_3(),
        CGAL::Las::Property::X(),
        CGAL::Las::Property::Y(),
        CGAL::Las::Property::Z())))
    {
      std::cerr << "Error: cannot read file " << fname << std::endl;
      return EXIT_FAILURE;
    }

  for (std::size_t i = 0; i < points.size(); ++ i)
    std::cout << points[i] << std::endl;
  
  return EXIT_SUCCESS;
}
