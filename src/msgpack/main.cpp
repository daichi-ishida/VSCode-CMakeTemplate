#include <msgpack.hpp>

#include <iostream>
#include <fstream>

namespace LOG
{
    struct Residual
    {
        Residual() = default;
        Residual(float _time, float _residual): time(_time), residual(_residual){}
        MSGPACK_DEFINE(time, residual);

        float time;
        float residual;
    };
}

int WriteAllBytes(const std::string& path, const char* buf, int size)
{
    std::ofstream ofs(path.c_str(), std::ios::out | std::ios::binary | std::ios::trunc );
     
    if(!ofs)
    {
        fprintf(stderr, "Cannot open!\n");
    }
     
    ofs.write(buf, size);
    ofs.close();
     
    return 0;
}

int ReadAllBytes(const std::string& path, std::vector<char>& buf)
{
    std::ifstream fin(path.c_str(), std::ios::in | std::ios::binary);
     
    if(!fin)
    {
        fprintf(stderr, "Cannot open!\n");
    }
    
    // Stop eating new lines in binary mode!!!
    fin.unsetf(std::ios::skipws);

    // get vector size:
    std::streampos fileSize;
    fin.seekg(0, std::ios::end);
    fileSize = fin.tellg();
    fin.seekg(0, std::ios::beg);

    // reserve capacity
    buf.reserve(fileSize);

    // read the data:
    buf.insert(buf.begin(),
               std::istream_iterator<char>(fin),
               std::istream_iterator<char>());
    fin.close();
     
    return 0;
}

void save()
{
    std::vector< LOG::Residual > log_residual;
    log_residual.emplace_back(4.2f, 1.0f);
    log_residual.emplace_back(9.6f, 0.82f);
    log_residual.emplace_back(13.6f, 0.6f);

    std::ofstream ofs("src/msgpack/log_residual.bin", std::ios::out | std::ios::binary | std::ios::trunc);
    msgpack::pack(ofs, log_residual);
}

void load()
{
    std::vector<char> packData;
    ReadAllBytes("src/msgpack/log_residual.bin", packData);

    msgpack::object_handle oh = msgpack::unpack(packData.data(), packData.size());

    msgpack::object obj = oh.get();
    std::vector< LOG::Residual > log_residual;
    obj.convert(log_residual);

    for(auto& elem : log_residual)
    {
        std::cout << elem.time << " : " << elem.residual << "\n";
    }
}

int main()
{
    // save();
    load();
}