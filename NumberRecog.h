/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//
// Copyright (C) 2015 MINAGAWA Takuya.
// Third party copyrights are property of their respective owners.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//M*/

#ifndef __NUMBER_RECOG__
#define __NUMBER_RECOG__

#include <opencv2/core/core.hpp>

namespace ccnr{

class NumberRecog
{
public:
	NumberRecog(void);
	~NumberRecog(void);

	///// One-vs-One Prediction ///////
	int Load(const std::string& train_file);
	int predict(const cv::Mat& feature) const ;
	cv::Mat score(const cv::Mat& feature) const;

	static cv::Mat HomogeneousVector(const cv::Mat& feature);

	////// Binary Class Prediction //////
	int LoadDetector(const std::string& train_file, const cv::Size& filter_size);

	void CharExistingCost(const std::vector<cv::Mat>& feature_map, cv::Mat& pos_cost_map, cv::Mat& neg_cost_map) const{
		cv::Mat response_map;
		ScoreMap(feature_map, response_map);
		Score2Cost(response_map, pos_cost_map, neg_cost_map);
	};

	////// One-vs-Rest  ////////
	int LoadOVR(const std::string& train_file, const cv::Size& filter_size);

	void CharExistingCostOVR(const std::vector<cv::Mat>& feature_map, cv::Mat& pos_cost_map, cv::Mat& neg_cost_map) const{
		std::vector<cv::Mat> response_map;
		ScoreMapOVR(feature_map, response_map);
		Score2CostOVR(response_map, pos_cost_map, neg_cost_map);
	};

private:
	/*! SVM�W��
	one-vs-one
	45�s�i0 vs 1, 0 vs 2, ..., 0 vs 9, 1 vs 2, ..., 8 vs 9�j
	�Ō��1�񂪃o�C�A�X
	*/
	cv::Mat _SvmCoeffs;	
	int _NumClass;

	//! �w�i�ƕ����̂Q�l���ޗpSVM�W��
	double _Bias;
	std::vector<cv::Mat> _Filters;

	//! One-vs-Rest SVM
	std::vector<double> _BiasOVR;
	std::vector<std::vector<cv::Mat> > _FilterOVR;

	//! SVM�̌W�����t�B���^�`���ɕϊ�
	static int SvmCoeff2Filters(const cv::Mat& svm_coeff, const cv::Size& filter_size, std::vector<cv::Mat>& filters, double& bias, int type = -1);

	//! SVM�̌W�����t�B���^�`���ɕϊ�
	static int SvmCoeff2Filters(const cv::Mat& svm_coeff, const cv::Size& filter_size, std::vector<std::vector<cv::Mat> >& filters, std::vector<double>& bias, int type = -1);

	//! �摜�����Ɋw�K�t�B���^�������ĉ��������߂�
	static void ScoreMap(const std::vector<cv::Mat>& feature_map, cv::Mat& response_map, const std::vector<cv::Mat>& filter, double bias);

	void ScoreMap(const std::vector<cv::Mat>& feature_map, cv::Mat& response_map) const{
		ScoreMap(feature_map, response_map, _Filters, _Bias);
	};

	//! �摜�����Ɋw�K�t�B���^�������ĉ��������߂�
	void ScoreMapOVR(const std::vector<cv::Mat>& feature_map, std::vector<cv::Mat>& response_map) const;

	//! SVM�������R�X�g�֕ϊ�
	static void Score2Cost(const cv::Mat& response_map, cv::Mat& pos_cost_map, cv::Mat& neg_cost_map);

	static void Score2CostOVR(const std::vector<cv::Mat>& response_map, cv::Mat& pos_cost_map, cv::Mat& neg_cost_map);
};

}

#endif