#include <iostream>

#include "sparsebase/sparse_format.h"
#include "sparsebase/sparse_converter.h"

using namespace std;
using namespace sparsebase;

int main(){

    int row[6] = {0, 0, 1, 1, 2, 2};
    int col[6] = {0, 1, 1, 2, 3, 3};
    int vals[6] = {10, 20, 30, 40, 50, 60};

    format::COO<int,int,int>* coo = new format::COO<int,int,int>(6, 6, 6, row, col, vals);

    auto converter = new utils::SparseConverter<int,int,int>();
    auto csr = converter->Convert(coo,CSR<int, int, int>::get_format_id_static());
    auto csr2 = csr->As<CSR>();

    auto dims = csr2->get_dimensions();
    int n = dims[0];
    int m = dims[1];
    int nnz = csr->get_num_nnz();

    cout << "CSR" << endl;

    for(int i=0; i<nnz; i++)
        cout << csr2->get_vals()[i] << ",";
    cout << endl;

    for(int i=0; i<nnz; i++)
        cout << csr2->get_col()[i] << ",";
    cout << endl;
    
    for(int i=0; i<n+1; i++)
        cout << csr2->get_row_ptr()[i] << ",";
    cout << endl;
    
    cout << endl;

    auto coo2 = converter->Convert(csr,COO<int, int, int>::get_format_id_static());

    auto coo3 = coo2->As<COO>();

    cout << "COO" << endl;

    for(int i=0; i<nnz; i++)
        cout << coo3->get_vals()[i] << ",";
    cout << endl;

    for(int i=0; i<nnz; i++)
        cout << coo3->get_row()[i] << ",";
    cout << endl;
    
    for(int i=0; i<nnz; i++)
        cout << coo3->get_col()[i] << ",";
    cout << endl;

    delete coo;
    delete converter;
    delete csr2;
    delete coo3;
}

