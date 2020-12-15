#include <H5Cpp.h>

#include <vector>
#include <iostream>

namespace LOG
{
    struct Table
    {
        Table() = default;
        Table(float _x, float _y): x(_x), y(_y){}

        float x;
        float y;
    };
}

const std::string filename("src/HDF5/log_residual.h5");

const std::string dataset_name = "Residuals";

const std::string label_x = "Time";
const std::string label_y = "Residual";

template<typename T>
void save()
{
    std::vector< T > log_residual;
    log_residual.emplace_back(4.2f, 1.0f);
    log_residual.emplace_back(9.6f, 0.82f);
    log_residual.emplace_back(13.6f, 0.6f);

    // the array of each length of multidimentional data.
    hsize_t dim[1];
    dim[0] = log_residual.size();

    // the length of dim
    int rank = sizeof(dim) / sizeof(hsize_t);

    // defining the datatype to pass HDF55
    H5::CompType mtype(sizeof(T));
    mtype.insertMember(label_x, HOFFSET(T, x), H5::PredType::NATIVE_FLOAT);
    mtype.insertMember(label_y, HOFFSET(T, y), H5::PredType::NATIVE_FLOAT);

    // preparation of a dataset and a file.
    H5::H5File file(filename, H5F_ACC_TRUNC);
    H5::DataSpace dataspace(rank, dim);
    H5::DataSet dataset = file.createDataSet(dataset_name, mtype, dataspace);

    // write
    dataset.write(log_residual.data(), mtype);
}

template<typename T>
void load()
{
    // preparation of a dataset and a file.
	H5::H5File file(filename, H5F_ACC_RDONLY);
	H5::DataSet dataset = file.openDataSet(dataset_name); 
	H5::DataSpace dataspace = dataset.getSpace();

    // defining the datatype to pass HDF55
    H5::CompType mtype(sizeof(T));
    mtype.insertMember(label_x, HOFFSET(T, x), H5::PredType::NATIVE_FLOAT);
    mtype.insertMember(label_y, HOFFSET(T, y), H5::PredType::NATIVE_FLOAT);

    int rank = dataspace.getSimpleExtentNdims();
    hsize_t dims_out[1];
    int ndims = dataspace.getSimpleExtentDims( dims_out, NULL);

    std::vector< T > log_residual;
    log_residual.resize(dims_out[0]);

    // read
    dataset.read(log_residual.data(), mtype);

    for(auto& elem : log_residual)
    {
        std::cout << elem.x << " : " << elem.y << "\n";
    }
}


int main()
{
    // save<LOG::Table>();
    load<LOG::Table>();
}