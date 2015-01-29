/* Johann Hauswald
 * jahausw@umich.edu
 * 2014
 */

#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include "opencv2/core/core.hpp"
#include "opencv2/core/types_c.h"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/gpu.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/stitching/stitcher.hpp"

using namespace cv;
using namespace std;

vector<Mat> segs;
vector<vector<KeyPoint> > keys;
FeatureDetector *detector = new SurfFeatureDetector();
DescriptorExtractor *extractor = new SurfDescriptorExtractor();
int iterations;

float calculateMiliseconds(timeval t1, timeval t2) {
  float elapsedTime;
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
  return elapsedTime;
}

vector<KeyPoint> exec_feature(const Mat &img) {
  vector<KeyPoint> keypoints;
  detector->detect(img, keypoints);

  return keypoints;
}

Mat exec_desc(const Mat &img, vector<KeyPoint> keypoints) {
  Mat descriptors;

  extractor->compute(img, keypoints, descriptors);

  descriptors.convertTo(descriptors, CV_32F);

  return descriptors;
}

int main(int argc, char **argv) {
  // data
  float runtimefeat = 0;
  float runtimedesc = 0;
  struct timeval t1, t2;

  // Generate test keys
  Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  if (img.empty()) {
    printf("image not found\n");
    exit(-1);
  }

  gettimeofday(&t1, NULL);
  vector<KeyPoint> key = exec_feature(img);
  gettimeofday(&t2, NULL);
  runtimefeat = calculateMiliseconds(t1, t2);

  gettimeofday(&t1, NULL);
  Mat testDesc = exec_desc(img, key);
  gettimeofday(&t2, NULL);
  runtimedesc = calculateMiliseconds(t1, t2);

  // Clean up
  delete detector;
  delete extractor;

  printf("SURF FE CPU Time=%4.3f ms\n", runtimefeat);
  printf("SURF FD CPU Time=%4.3f ms\n", runtimedesc);

  return 0;
}