#include "MappingParameters.h"

//debug
#include <iostream>
using std::cout;
using std::endl;
//end debug

MappingParameters::MappingParameters(void)
{
	scale = 1.0;
	rotation = translationX = translationY = 0.0;
}


MappingParameters::~MappingParameters(void)
{
}

void MappingParameters::caculateNewCoordinates(double x, double y, double &resX, double &resY){
	double SCosR = scale * cos(rotation), SSinR = scale * sin(rotation);
	resX = x * SCosR - y * SSinR + translationX;
	resY = x * SSinR + y * SCosR + translationY;
}

void MappingParameters::inverse(){
	scale = 1.0 / scale;
	rotation = -rotation;
}

void MappingParameters::getMappingMatrix(cv::Mat &mappingMatrix){
	//return a matrix like
	//			[ScosR	-SsinR]
	//			[SsinR	 ScosR]
	double SCosR = scale * cos(rotation), SSinR = scale * sin(rotation);
	mappingMatrix = (cv::Mat_<double>(2, 2) << SCosR, -SSinR,
												SSinR, SCosR);
}

void MappingParameters::getTranslationMatrix(cv::Mat &translationMatrix){
	//return a matrix like
	//					[tx 0]
	//					[0 ty]
	translationMatrix = (cv::Mat_<double>(2, 2) << translationX, 0,
													0, translationY);
}

void MappingParameters::getAlignedXY(const cv::Mat &shapeX, const cv::Mat &shapeY, 
	cv::Mat &newShapeX, cv::Mat &newShapeY, const double transRatio){
	//newShapeX & newShapeY should be created before
	//this function will copy each element to the newShapeX & newShapeY
	const int numberOfPoints = shapeX.rows;

	cv::Mat _allOneMat(numberOfPoints, 2, CV_64F, cv::Scalar::all(1));
	cv::Mat _shapeXY(numberOfPoints, 2, CV_64F);
	shapeX.col(0).copyTo(_shapeXY.col(0));
	shapeY.col(0).copyTo(_shapeXY.col(1));
	
	//debug
	//cout << "shapeX.col(0)" << endl << shapeX.col(0) << endl;
	//cout << "_shapeXY" << _shapeXY << endl;
	//end debug

	cv::Mat _mappingMat, _translationMat;
	getMappingMatrix(_mappingMat);
	getTranslationMatrix(_translationMat);
	//debug
	//cout << "_mappingMat" << _mappingMat << endl;
	//cout << "_translationMat" << _translationMat << endl;
	//end debug

	cv::Mat _resMat = _shapeXY * _mappingMat.t() + transRatio * _allOneMat * _translationMat;
	_resMat.col(0).copyTo(newShapeX);
	_resMat.col(1).copyTo(newShapeY);
}