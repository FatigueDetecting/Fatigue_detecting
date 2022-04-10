// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*

    This example program shows how to find frontal human faces in an image and
    estimate their pose.  The pose takes the form of 68 landmarks.  These are
    points on the face such as the corners of the mouth, along the eyebrows, on
    the eyes, and so forth.  
    
    .0


    The face detector we use is made using the classic Histogram of Oriented
    Gradients (HOG) feature combined with a linear classifier, an image pyramid,
    and sliding window detection scheme.  The pose estimator was created by
    using dlib's implementation of the paper:
       One Millisecond Face Alignment with an Ensemble of Regression Trees by
       Vahid Kazemi and Josephine Sullivan, CVPR 2014
    and was trained on the iBUG 300-W face landmark dataset (see
    https://ibug.doc.ic.ac.uk/resources/facial-point-annotations/):  
       C. Sagonas, E. Antonakos, G, Tzimiropoulos, S. Zafeiriou, M. Pantic. 
       300 faces In-the-wild challenge: Database and results. 
       Image and Vision Computing (IMAVIS), Special Issue on Facial Landmark Localisation "In-The-Wild". 2016.
    You can get the trained model file from:
    http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2.
    Note that the license for the iBUG 300-W dataset excludes commercial use.
    So you should contact Imperial College London to find out if it's OK for
    you to use this model file in a commercial product.


    Also, note that you can train your own models using dlib's machine learning
    tools.  See train_shape_predictor_ex.cpp to see an example.

    


    Finally, note that the face detector is fastest when compiled with at least
    SSE2 instructions enabled.  So if you are using a PC with an Intel or AMD
    chip then you should enable at least SSE2 instructions.  If you are using
    cmake to compile this program you can enable them by using one of the
    following commands when you create the build project:
        cmake path_to_dlib_root/examples -DUSE_SSE2_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_SSE4_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_AVX_INSTRUCTIONS=ON
    This will set the appropriate compiler options for GCC, clang, Visual
    Studio, or the Intel compiler.  If you are using another compiler then you
    need to consult your compiler's manual to determine how to enable these
    instructions.  Note that AVX is the fastest but requires a CPU from at least
    2011.  SSE4 is the next fastest and is supported by most current machines.  
*/


#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <iostream>
#include <typeinfo>
#include <math.h>                             //为绝对值fabs所加

using namespace dlib;
using namespace std;

// ----------------------------------------------------------------------------------------
float Standard(full_object_detection shape) {
    float StandardWink = 0;
    float StandardYaw = 0;
    float StandardNode = 0;
    StandardWink = fabs((shape.part(37).y() + shape.part(38).y()) - (shape.part(41).y() + shape.part(42).y())) / (2 * fabs(shape.part(39).x() - shape.part(36).x()));
    StandardYaw = fabs((shape.part(31).y() + shape.part(35).y()) / 2 - shape.part(37).y()) / fabs((shape.part(31).y() + shape.part(35).y()) / 2 - (shape.part(48).y() + shape.part(54).y()) / 2);
    StandardNode = shape.part(33);
    float StandardTotal = { StandardWink, StandardYaw,StandardNode };
    return StandardTotal;
}

bool Wink(full_object_detection shape, float StandardWink) {
    float UpperEyebrow = 0;
    float LowerEyebrow = 0;
    float EyeVertical = 0;
    float EyeHorizontal = 0;
    int WinkRatio = 0;
  
    UpperEyebrow = (shape.part(37).y() + shape.part(38).y());
    LowerEyebrow = (shape.part(41).y() + shape.part(42).y());
    EyeVertical = fabs(UpperEyebrow - LowerEyebrow) / 2;
    EyeHorizontal = fabs(shape.part(39).x() - shape.part(36).x());
    WinkRatio = ((EyeVertical / EyeHorizontal) > StandardWink);  //大于阈值为正常，小于阈值为疲劳眨眼
    return WinkRatio;
};


bool Yaw(full_object_detection shape, float StandardYaw) {
    float k = 0, MNSO = 0;
    float YawRatio;
    k = fabs((shape.part(31).y() + shape.part(35).y()) / 2 - shape.part(37).y());
    MNSO = fabs((shape.part(31).y() + shape.part(35).y()) / 2 - (shape.part(48).y() + shape.part(54).y()) / 2);
    YawRatio = fabs(k / MNSO) < StandardYaw;	//小于阈值为正常，大于阈值为疲劳打哈欠
    return YawRatio;
};

bool Node(full_object_detection shape, float StandardNode) {
    float r = 0, Pich = 0;
    float NodeRatio = 0;
    NodeRatio = fabs((shape.part(33).y() - StandardNode) / StandardNode) > 0.2;	//鼻尖连点
    return NodeRatio;
};
//-------------------------------------------------------------------------------
int main(int argc, char** argv)
{  
    try
    {
        // This example takes in a shape model file and then a list of images to
        // process.  We will take these filenames in as command line arguments.
        // Dlib comes with example images in the examples/faces folder so give
        // those as arguments to this program.
        if (argc == 1)
        {
            cout << "Call this program like this:" << endl;
            cout << "./face_landmark_detection_ex shape_predictor_68_face_landmarks.dat faces/*.jpg" << endl;
            cout << "\nYou can get the shape_predictor_68_face_landmarks.dat file from:\n";
            cout << "http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
            return 0;
        }

        // We need a face detector.  We will use this to get bounding boxes for
        // each face in an image.
        frontal_face_detector detector = get_frontal_face_detector();
        // And we also need a shape_predictor.  This is the tool that will predict face
        // landmark positions given an image and face bounding box.  Here we are just
        // loading the model from the shape_predictor_68_face_landmarks.dat file you gave
        // as a command line argument.
        shape_predictor sp;
        deserialize(argv[1]) >> sp;


        image_window win, win_faces;
        // Loop over all the images provided on the command line.
        for (int i = 2; i < argc; ++i)
        {
            cout << "processing image " << argv[i] << endl;
            array2d<rgb_pixel> img;
            load_image(img, argv[i]);
            // Make the image larger so we can detect small faces.
            pyramid_up(img);

            // Now tell the face detector to give us a list of bounding boxes
            // around all the faces in the image.
            std::vector<rectangle> dets = detector(img);
            cout << "Number of faces detected: " << dets.size() << endl;

            // Now we will go ask the shape_predictor to tell us the pose of
            // each face we detected.
            std::vector<full_object_detection> shapes; 
            int SumNode = 0;
            int SumWink = 0;
            int SumYaw = 0;
            bool NodeDetermine, YawDetermine, WinkDetermine;
            float SumStandard[3] = {0,0,0};
            //-----------------------前十张即为标准值---------------------------------------------------
            for (unsigned long j = 0; j < 10; ++j) {
                full_object_detection shape = sp(img, dets[j]);
                SumStandard[1] = SumStandard[1] + Standard(shape).StandarWink;
                SumStandard[2] = SumStandard[2] + Standard(shape).StandarYaw;
                SumStandard[3] = SumStandard[3] + Standard(shape).StandarNode;
            }
            float StandardWink = SumStandard[1] / 10;
            float StandardYaw = SumStandard[2] / 10;
            float StandardNode = SumStandard[3] / 10;
            //------------------------------------------------------------------------------------------------
            for (unsigned long j = 0; j < dets.size(); ++j)
            {
                full_object_detection shape = sp(img, dets[j]);
                cout << "number of parts: "<< shape.num_parts() << endl;
               
                //--------------------------点头部分连续五次发生----------------------------------------------------
                if (Node(shape,StandardNode).NodeRatio == 1) {
                    SumNode = SumNode + 1;
                    if (SumNode > 5) {
                        NodeDetermine = true;
                    }
                }
                else {
                    SumNode = 0;
                }
                //--------------------------------------------------------------------------------------------------------
                //--------------------------眨眼部分连续十次发生--------------------------------------------------------
                if (Wink(shape,StandardWink).WinkRatio == 1) {    
                    SumWink = SumWink + 1;
                    if (SumWink > 10) {
                        WinkDetermine = true;
                    }
                }
                else {
                    SumNode = 0;
                }
                
                //------------------------------------------------------------------------------------------------------
                //-------------------------哈欠连续二十次发生----------------------------------------------------------
                if (Yaw(shape,StandardYaw).YawRatio == 1) {
                    SumYaw = SumYaw + 1;
                    if (SumYaw > 20) {
                        YawDetermine = true;
                    }
                }
                else {
                    SumYaw = 0;
                }

                //-----------------------------------------------------------------------------------------------------
                /*for (int i = 0; i < 68; i++)
                { 
                    cout << "pixel position of "<< i << " part:  " << shape.part(i) << endl;
                }*/
                //cout << "pixel position of second part: " << shape.part(1) << endl;
                // You get the idea, you can get all the face part locations if
                // you want them.  Here we just store them in shapes so we can
                // put them on the screen.
                cout << shape.part() << endl; 
                cout << "Wink Fatigue is" << WinkDetermine << endl;
                cout << "Yaw Fatigue is" << YawDetermine << endl;
                cout << "Node Fatigue is" << NodeDetermine << endl;
                shapes.push_back(shape);
            }


            // Now let's view our face poses on the screen.
            win.clear_overlay();
            win.set_image(img);
            win.add_overlay(render_face_detections(shapes));

            // We can also extract copies of each face that are cropped, rotated upright,
            // and scaled to a standard size as shown here:
            dlib::array<array2d<rgb_pixel >> face_chips;
            extract_image_chips(img, get_face_chip_details(shapes), face_chips);
            win_faces.set_image(tile_images(face_chips));
            
            cout << "Hit enter to process the next image..." << endl;
            cin.get();
        }
    }
    catch (exception& e)
    {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
}

// ----------------------------------------------------------------------------------------

