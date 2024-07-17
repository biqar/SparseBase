#include <iostream>
#include <set>

#include "sparsebase/format/csr.h"
#include "sparsebase/format/format_order_one.h"
#include "sparsebase/format/format_order_two.h"
#include "sparsebase/object/object.h"
#include "sparsebase/reorder/gray_reorder.h"
#include "sparsebase/bases/iobase.h"
#include "sparsebase/bases/reorder_base.h"

using namespace std;
using namespace sparsebase;
using namespace io;
using namespace bases;
using namespace format;
using namespace reorder;

using vertex_type = int;
using edge_type = int;
using value_type = float;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Usage: ./gray_order <uedgelist_file> <reorder_output_file>\n";
    cout
        << "Hint: You can use the edgelist: examples/data/com-dblp.uedgelist\n";
    return 1;
  }
  cout << "F t re  s sp r e!" << endl;
  string file_name = argv[1];
  string out_file_name = argv[2];

  cout << "********************************" << endl;
  cout << "Reading graph from " << file_name << "..." << endl;

  object::Graph<vertex_type, edge_type, value_type> g;
  g.ReadConnectivityFromEdgelistToCSR(file_name);
  cout << "Number of vertices: " << g.n_ << endl;
  cout << "Number of edges: " << g.m_ << endl;

  cout << "********************************" << endl;

  cout << "Generating Gray ordering..." << endl;

  auto *con =
      g.get_connectivity()
          ->AsAbsolute<format::CSR<vertex_type, edge_type, value_type>>();
  vertex_type num_rows = con->get_dimensions()[0];
  vertex_type num_columns = con->get_dimensions()[0];
  edge_type num_non_zeros = con->get_num_nnz();

  std::cout << "Matrix has "
            << num_rows << " rows, "
            << num_columns << " columns, and "
            << num_non_zeros << " non-zeros" << std::endl;

  // Context representing the GPU with ID 0 in the system
  context::CPUContext cpu_context;

  // Create a parameters object to store special parameters specific
  GrayReorderParams gray_params(BitMapSize::BitSize16, 20, ((num_non_zeros / num_rows) / BitMapSize::BitSize16));

  // Gray reordering
  vertex_type * gray_reorder = bases::ReorderBase::Reorder<GrayReorder>(gray_params, con, {&cpu_context}, true);

  cout << "********************************" << endl;

  cout << "Checking the correctness of the ordering..." << endl;
  bool order_is_correct = true;
  set<vertex_type> ids;
  for (vertex_type i = 0; i < num_rows && order_is_correct; i++) {
    vertex_type i_order = gray_reorder[i];
    if (i_order < num_rows && ids.find(i_order) == ids.end()) {
      ids.insert(i_order);
    } else {
      cout << "Gray ordering is incorrect!";
      order_is_correct = false;
      return 1;
    }
  }
  if (ids.size() > num_rows) {
    cout << "Gray ordering is incorrect!";
    order_is_correct = false;
  }
  if (order_is_correct) {
    cout << "Order is correct!" << endl;
  } else {
    cout << "Gray ordering is incorrect!";
    order_is_correct = false;
    return 1;
  }

  // Save the result in file
  std::ofstream gray_order_file(out_file_name.c_str());
  assert(gray_order_file.is_open() && "Gray order output file is not open!");
  std::copy(&gray_reorder[0], &gray_reorder[num_rows], std::ostream_iterator<vertex_type>(gray_order_file, "\n"));
  gray_order_file.close();

  delete gray_reorder;

  return 0;
}
