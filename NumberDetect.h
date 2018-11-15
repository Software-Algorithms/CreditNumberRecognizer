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

#ifndef __NUMBER_DETECT__
#define __NUMBER_DETECT__

#include <opencv2/core/core.hpp>

namespace ccnr{

class NumberDetect
{
public:
	NumberDetect(void);
	~NumberDetect(void);

	typedef int CHAR_EDGE_TYPE;

	static const CHAR_EDGE_TYPE
		CHAR_BLANK = 0,	// �����ƕ����̊�
		CHAR_LEFT = 1,	//�����̉E�[
		CHAR_RIGHT = 2,	//�����̍��[
		CHAR_STRING_LEFT = 3,	// ������S�̂̉E�[�i�n�_�j
		CHAR_STRING_RIGHT = 4;	// ������S�̂̍��[�i�I�_�j
	static const int CHAR_EDGE_TYPE_NUM = 5;

	typedef enum{
		TYPE4444,
		TYPE465,
		TYPE464
	}CREDIT_PATTERN;

	float _char_aspect_ratio;	// �����̃A�X�y�N�g��
	float _char_width_div;	// �����̋�؂�ʒu����ɑ΂���y�i���e�B
	float _min_char_height_ratio;	// �摜�̕��ɑ΂���ŏ����������̔�
	float _max_char_height_ratio;	// �摜�̕��ɑ΂���ő啶�������̔�

	//! �N���W�b�g�J�[�h�ԍ��̈ʒu���擾
	void ExtractNumbers(const cv::Mat& edge_img, std::vector<cv::Rect>& num_pos, CREDIT_PATTERN& pattern) const;

	/////////////////////////////////////
	//! �N���W�b�g�J�[�h�ԍ���̈ʒu���擾
	static void DetectStringHeight(const cv::Mat& edge_img, std::vector<cv::Rect>& candidates, int min_char_height, int max_char_height);

	/////////////////////////////////////

	//! �A�s�A�����X�Ɋ�Â����R�X�g�֐��̐���
	static void CreateAppearanceCosts(const cv::Mat& edge_img, std::vector<std::vector<double> >& app_costs);

	//! �����Ԃ̋�؂�ʒu�Ɋ�Â����R�X�g�֐��̐����i���������j
	static void CreateRegularizationCosts(std::vector<double>& reg_costs, int window_size, double sigma);


	//! �܂�������̗��[���Z�o�������ƂŁA�̈���ϓ����肵�Ă��ꂼ��ōœK�ȏꏊ���Z�o
	/*!
	\param[out] char_breaks �����̋�؂�ʒu
	\param[in] app_costs �����ڃx�[�X�̃R�X�g�֐�
	\param[in] pos_costs �����̈ʒu�Y���̃R�X�g�֐�
	\param[in] avg_string_len ������̒����̕���
	\param[in] sring_len_div ������̒����̕W���΍�
	\paran[in] char_pattern ��؂蕶���p�^�[��
	*/
	static double ExtractCharRange(std::vector<CHAR_EDGE_TYPE>& char_breaks, const std::vector<std::vector<double> >& app_costs,
		const std::vector<double>& pos_costs, float avg_string_len, float string_len_div,
		const std::vector<int>& char_pattern, double init_cost = 10000);

	//! �N���W�b�g�J�[�h�ԍ��̃p�^�[�����擾
	static void CreateCreditBreakPattern(std::vector<CHAR_EDGE_TYPE>& pattern, CREDIT_PATTERN type = TYPE4444);
	static void ConvertXtoRects(const std::vector<int>& breaks, std::vector<cv::Rect>& number_rects, 
		const cv::Rect& region, const CREDIT_PATTERN& pattern);

private:
	std::vector<CREDIT_PATTERN> _PATTERN_TYPES;
	std::vector<std::vector<CHAR_EDGE_TYPE> > _CHAR_BREAK_PATTERNS;

	//! �J�[�h�ԍ��̂���s���當���Ԃ̋�؂�ʒu���Z�o
	/*!
	\param[in] edge_img �G�b�W�摜
	\param[in] number_area ������̈�
	\param[out] break_pos �����̋�؂�ʒu
	\param[out] pattern �N���W�b�g�J�[�h�ԍ��̕��ѕ��i4-4-4-4, 4-6-5, 4-6-4�j
	\param[in] min_cost �ŏ��R�X�g�B�v�Z�̑��؂�Ɏg�p�B
	\return �ŏ��R�X�g�B�������قǁu������ۂ��v�B
	*/
	double DetectCharacterRange(const cv::Mat& edge_img, const cv::Rect& number_area, std::vector<int>& break_pos, CREDIT_PATTERN& pattern, double min_cost = 10000) const;

	//! �J�[�h�ԍ��̂���s���當���Ԃ̋�؂�ʒu���Z�o
	/*!
	\param[in] edge_img �G�b�W�摜
	\param[in] number_area ������̈�
	\param[out] char_boxes �����̈�
	\param[out] pattern �N���W�b�g�J�[�h�ԍ��̕��ѕ��i4-4-4-4, 4-6-5, 4-6-4�j
	\return �ŏ��R�X�g�B�������قǁu������ۂ��v�B
	*/
	double DetectCharacterBoxes(const cv::Mat& edge_img, const std::vector<cv::Rect>& number_area, std::vector<cv::Rect>& char_boxes, CREDIT_PATTERN& pattern) const;

	//! �N���W�b�g�J�[�h�ԍ��̃p�^�[�����擾
	//static void CreateCreditBreakPattern(std::vector<int>& pattern, CREDIT_PATTERN type = TYPE4444);

	//! �R�X�g�}�b�v����
	static void CreateCharLeftCost(const cv::Mat& derivmap, std::vector<double>& char_left_cost, int slide = 1);
	static void CreateCharRightCost(const cv::Mat& derivmap, std::vector<double>& char_right_cost, int slide = 1);
	static void CreateCharStringLeftCost(const cv::Mat& block_deriv, const cv::Mat& integ, std::vector<double>& char_string_left_cost, int slide = 1);
	static void CreateCharStringRightCost(const cv::Mat& block_deriv, const cv::Mat& integ, std::vector<double>& char_string_right_cost, int slide = 1);
	//static void CreateCharStringLeftCost(const std::vector<double>& char_left_cost, const cv::Mat& integ, std::vector<double>& char_string_left_cost);
	//static void CreateCharStringRightCost(const std::vector<double>& char_right_cost, const cv::Mat& integ, std::vector<double>& char_string_right_cost);
	static void CreateCharBlankCost(const cv::Mat& derivmap, std::vector<double>& char_blank_cost);

	//! ���z�i�����j
	static void CreateDeriv(const cv::Mat& prj, cv::Mat& div1st, cv::Mat& div2nd);

	//! �u���b�N���̌��z
	static void CreateBlockDeriv(const cv::Mat& prj, int block_size, cv::Mat& dst);

	// ���ʂ��i�[
	//static void ConvertXtoRects(const std::vector<int>& breaks, std::vector<cv::Rect>& number_rects, 
	//	const cv::Rect& region, const CREDIT_PATTERN& pattern);

	static void InitPositions(const std::vector<double>& app_costs, std::vector<double>& target_costs, std::vector<int>& positions);

	static void MinScorePositions(const std::vector<double>& app_costs, const std::vector<double>& size_costs, 
		int pos, double* min_cost, int* min_position);

	//! �N���W�b�g�J�[�h�ԍ���̖ޓx�]��
	static void EvaluateNumberStrings(const std::vector<std::pair<int,int> >& line_pos, std::vector<double>& scores, const std::vector<float>& prj);
};


}
#endif
