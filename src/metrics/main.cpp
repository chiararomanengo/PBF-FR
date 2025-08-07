#include "io/read_GIS.h"

#include <liblas/liblas.hpp>

#include <cinolib/meshes/meshes.h>
#include <cinolib/merge_meshes_at_coincident_vertices.h>

#ifdef USE_CINOLIB_GUI
#include <cinolib/gl/glcanvas.h>
#include <cinolib/gl/surface_mesh_controls.h>
#endif

#include <tclap/CmdLine.h>

#include <filesystem>

namespace fs = std::filesystem;

const double point_size=2.5;

int main(int argc, char *argv[])
{
    std::string las_directory ;
    std::string our_directory;
    std::string las_filename ;
    std::string facade_filename ;

    // std::vector<cinolib::Polygonmesh<>> pcs;

    try
    {
        // Define command line parser and arguments
        TCLAP::CmdLine cmd("PBF-FR-metrics", ' ', "version 0.5");

        // Define main functionalities options
        TCLAP::ValueArg<std::string> las_arg("l", "las", "LAS Dir ", true, "name_las", "string", cmd);
        TCLAP::ValueArg<std::string> facade_arg("f", "facade", "Facade Dir ", true, "name_fac", "string", cmd);

        // cmd.xorAdd( pavements_arg, pavements_gis_arg );

        // Parse the argv array
        cmd.parse(argc, argv);

        las_directory = las_arg.getValue();
        our_directory = facade_arg.getValue();
    }
    catch (TCLAP::ArgException &e) // catch exceptions
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        exit(-3);
    }

    int subfolderCount = 0;

    try {
        for (const auto& entry : fs::directory_iterator(las_directory)) {
            if (entry.is_directory()) {
                ++subfolderCount;
            }
        }
        std::cout << "Number of subfolders: " << subfolderCount << std::endl;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
    }

    uint TP_=0, TN_=0, FP_=0, FN_=0;

    for (uint d=0; d < subfolderCount; d++)
    {
        // las_filename = las_directory + "\\" + std::to_string(d) + "\\" + std::to_string(d) + "_Nov18_train_lastools_grouped.las.LAS";
        las_filename = las_directory + "\\" + std::to_string(d) + "\\" + std::to_string(d) + ".las";
        facade_filename = our_directory + "\\building" + std::to_string(d) + "\\label.txt";

        std::vector<bool> point_is_facade;
        std::vector<bool> point_is_facade_las;

        std::vector<cinolib::Color> colors;

        std::cout << "reading our... "  << facade_filename << std::endl;

        std::ifstream ifs_facade;
        ifs_facade.open(facade_filename, std::ios::in);

        if (!ifs_facade.is_open())
        {
            std::cerr << "error opening " << facade_filename << std::endl;
            continue;
        }

        uint is_facade;
        while (ifs_facade >> is_facade)
        {
            point_is_facade.push_back(is_facade);
        }

        ifs_facade.close();

        std::cout << "reading las... " << las_filename << std::endl;
        std::vector<cinolib::vec3d> las_points;

        std::ifstream ifs;
        ifs.open(las_filename, std::ios::in | std::ios::binary);

        if (!ifs.is_open())
        {
            std::cerr << "error opening " << las_filename << std::endl;
            exit(1);
        }

        liblas::ReaderFactory f;
        liblas::Reader reader = f.CreateWithStream(ifs);

        liblas::Header const& header = reader.GetHeader();

        // std::cout << "Compressed: " << ((header.Compressed() == true) ? "true":"false");
        // std::cout << "Signature: " << header.GetFileSignature() << '\n';
        // std::cout << "Points count: " << header.GetPointRecordsCount() << '\n';

        while (reader.ReadNextPoint())
        {
            liblas::Point const& p = reader.GetPoint();

            // get point classification integer
            uint8_t classification = p.GetClassification().GetClass();

            las_points.push_back(cinolib::vec3d(p.GetX(), p.GetY(), p.GetZ()));

            uint8_t r = static_cast<uint8_t>(p.GetColor().GetRed() / 256);
            uint8_t g = static_cast<uint8_t>(p.GetColor().GetGreen() / 256);
            uint8_t b = static_cast<uint8_t>(p.GetColor().GetBlue() / 256);

            colors.push_back(cinolib::Color(r / 255., g / 255., b / 255.));

            // std::cout << p.GetX() << ", " << p.GetY() << ", " << p.GetZ() << std::endl;

            if (static_cast<int>(classification) == 5)
                point_is_facade_las.push_back(true);
            else
                point_is_facade_las.push_back(false);;

            // std::cout << " :: " << static_cast<int>(classification) << std::endl;
        }

        ifs.close();

        uint TP=0, TN=0, FP=0, FN=0;

        for (uint i=0; i < las_points.size(); ++i)
        {
            if (point_is_facade_las[i] && point_is_facade[i])
                TP++;
            else if (!point_is_facade_las[i] && !point_is_facade[i])
                TN++;
            else if (point_is_facade_las[i] && !point_is_facade[i])
                FP++;
            else if (!point_is_facade_las[i] && point_is_facade[i])
                FN++;
        };

        double precision = (double) TP / (TP + FP);
        double recall = (double) TP / (TP + FN);
        double F1 = 2 * (precision * recall) / (precision + recall);

        std::cout << "BUILDING " << d << std::endl;
        std::cout << "----------------- " << std::endl;

        std::cout << "TP: " << TP << std::endl;
        std::cout << "TN: " << TN << std::endl;
        std::cout << "FP: " << FP << std::endl;
        std::cout << "FN: " << FN << std::endl;

        std::cout << "Precision: " << precision << std::endl;
        std::cout << "Recall: " << recall << std::endl;
        std::cout << "F1: " << F1 << std::endl;

        std::cout << "----------------- " << std::endl;
        std::cout << "----------------- " << std::endl;

        TP_ += TP;
        TN_ += TN;
        FP_ += FP;
        FN_ += FN;

        // pcs.push_back(cinolib::Polygonmesh<>());

        // for (uint i=0; i < las_points.size(); ++i)
        // {
        //     if (!point_is_facade_las[i] && !point_is_facade[i])
        //         continue;

        //     uint id = pcs.at(pcs.size()-1).vert_add(las_points[i]);

        //     if (point_is_facade_las[i] && point_is_facade[i])
        //         pcs.at(pcs.size()-1).vert_data(id).color = cinolib::Color(0, 1, 0); //TP++;
        //     else if (point_is_facade_las[i] && !point_is_facade[i])
        //         pcs.at(pcs.size()-1).vert_data(id).color = cinolib::Color(0, 0, 1); //FP++;
        //     else if (!point_is_facade_las[i] && point_is_facade[i])
        //         //pcs.at(pcs.size()-1).vert_data(id).color = colors.at(i);
        //         pcs.at(pcs.size()-1).vert_data(id).color = cinolib::Color(1, 0, 0); //FN++;
        //     else //if (!point_is_facade_las[i] && !point_is_facade[i])
        //         pcs.at(pcs.size()-1).vert_data(id).color = cinolib::Color(.9, .9, .9); //TN++;
        // };

        // pcs.at(pcs.size()-1).show_wireframe_width(point_size);
        // // pcs.at(pcs.size()-1).translate(-center);
        // pcs.at(pcs.size()-1).show_mesh_points();
        // pcs.at(pcs.size()-1).updateGL();
    }

    double precision_ = (double) TP_ / (TP_ + FP_);
    double recall_ = (double) TP_ / (TP_ + FN_);
    double F1_ = 2 * (precision_ * recall_) / (precision_ + recall_);

    std::cout << "----------------- " << std::endl;
    std::cout << "----------------- " << std::endl;
    std::cout << "----------------- " << std::endl;
    std::cout << "GLOBAL STATISTICS " << std::endl;
    std::cout << "----------------- " << std::endl;



    std::cout << "TP: " << TP_ << std::endl;
    std::cout << "TN: " << TN_ << std::endl;
    std::cout << "FP: " << FP_ << std::endl;
    std::cout << "FN: " << FN_ << std::endl;

    std::cout << "Precision: " << precision_ << std::endl;
    std::cout << "Recall: " << recall_ << std::endl;
    std::cout << "F1: " << F1_ << std::endl;


    std::cout << "----------------- " << std::endl;

    return 0;
}
