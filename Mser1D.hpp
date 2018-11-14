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

#ifndef __MSER1D__
#define __MSER1D__

#include <vector>
#include <algorithm>

namespace ccnr{

//! �e�������l�ɂ�����̈�̍\����
typedef struct{
	int pos;	// �J�n�ʒu
	int len;	// ����
	double threshold;	// �������l
	std::vector<int> child_idx;	// �q�m�[�h�i�������l�����傫�������ꍇ�j
}REGION_1D;


//! 1�����}�X�N��������
template<typename _T>
void Mask1D(const std::vector<_T>& histogram, std::vector<_T>& masked_histogram, const std::vector<unsigned char>& mask)
{
	if(mask.empty()){
		masked_histogram = histogram;
		return;
	}
	
	assert(mask.size() == histogram.size());

	std::vector<_T>::const_iterator min_it = std::min_element(histogram.begin(), histogram.end());

	masked_histogram.resize(histogram.size(), *min_it);
	int hist_len = histogram.size();
	for(int i = 0; i < hist_len; i++){
		if(mask[i] > 0){
			masked_histogram[i] = histogram[i];
		}
	}
}



//! �P�����z�񂩂炵�����l��ς��Ȃ���ċA�I�ɘA���̈�̓���q�\���𒊏o
/*!
\param[in] histogram 1�����z��
\param[out] regions �؍\�����������̈�
\param[in] rgn_id ���ݎQ�Ƃ��Ă���̈��ID�iregions�̃C���f�b�N�X�j
\param[in] step �������l���グ��X�e�b�v��
*/
template <typename _T>
void CreateMserRegionTree(const std::vector<_T>& histogram, std::vector<REGION_1D>& regions, 
	int rgn_id, double step)
{
	int start, end;
	double th;
	// ������Ԃ̏ꍇ
	if(regions.empty()){
		start = 0;
		end = histogram.size();
		std::vector<_T>::const_iterator min_it = std::min_element(histogram.begin(), histogram.end());
		th = (double)(*min_it) - 0.000001;
	}
	else{
		start = regions[rgn_id].pos;
		end = start + regions[rgn_id].len;
		th = regions[rgn_id].threshold + step;
	}
	for(int i=start; i<end; i++){
		if(histogram[i] >= th){
			REGION_1D rgn;
			rgn.pos = i;
			int count = 0;
			while(i<end && histogram[i] >= th){
				count ++;
				i++;
			}
			rgn.len = count;
			rgn.threshold = th;
			int new_rgn_id = regions.size();
			if(new_rgn_id > 0){
				regions[rgn_id].child_idx.push_back(new_rgn_id);
			}
			regions.push_back(rgn);

			CreateMserRegionTree(histogram, regions, new_rgn_id, step);
		}
	}
}



//! �؂𕪉����ăN���X�^�����O(���m�[�h�����P�ł���A���̈�)
/*!
\param[in] tree_regions �S�������l�ł̑S�̈�Ƃ��̖؍\��
\param[out] clustered_idx �؍\�����q�m�[�h���P�ƂȂ�̈�ŃN���X�^�����O����ID�i�������l�������̏��Ɂj
*/
void ClusterRegionsFromTree(const std::vector<REGION_1D>& tree_regions, std::vector<std::vector<int> >& clustered_idx)
{
	std::vector<bool> check(tree_regions.size(), false);
	for(int i=0; i<tree_regions.size(); i++){
		if(!check[i]){
			std::vector<int> idx_chain;
			idx_chain.push_back(i);
			check[i] = true;
			int j = i;
			while(tree_regions[j].child_idx.size() == 1){
				j = tree_regions[j].child_idx[0];
				idx_chain.push_back(j);
				check[j] = true;
			}
			clustered_idx.push_back(idx_chain);
		}
	}
}


//! �̈�̕ω��ʂ��Z�o
/*!
\param[in] tree_regions �S�������l�ł̑S�̈�Ƃ��̖؍\��
\param[out] area_variation clustered_idx�ɑΉ�����ʐς̕ω���
\param[in] clustered_idx �؍\�����q�m�[�h���P�ƂȂ�̈�ŃN���X�^�����O����ID�i�������l�������̏��Ɂj
*/
void AreaVariation(const std::vector<REGION_1D>& tree_regions, 
	std::vector<std::vector<double> >& area_variation, 
	const std::vector<std::vector<int> >& clustered_idx,
	int delta, int min_area, int max_area)
{
	std::vector<std::vector<int> >::const_iterator it, it_end = clustered_idx.end();
	for(it = clustered_idx.begin(); it != it_end; it++){
		std::vector<double> variation;
		if(it->size() < 2*delta + 1 || 
			tree_regions[it->front()].len < min_area || tree_regions[it->back()].len > max_area){
			area_variation.push_back(variation);
			continue;
		}
		
		for(int i=0; i<delta; i++){
			variation.push_back(-1);
		}
		int end = it->size() - delta;
		for(int i=delta; i<end; i++){
			int cand_idx = (*it)[i];
			double val = (double)(tree_regions[cand_idx-delta].len - tree_regions[cand_idx+delta].len) / tree_regions[cand_idx].len;
			variation.push_back(val);
		}
		for(int i=0; i<delta; i++){
			variation.push_back(-1);
		}
		area_variation.push_back(variation);
	}
}



//! �̈�̕ω��ʂ̋Ǐ��I�ɍŏ��ȃC���f�b�N�X���Z�o
/*!
\param[in] clustered_idx �؍\�����q�m�[�h���P�ƂȂ�̈�ŃN���X�^�����O����ID�i�������l�������̏��Ɂj
\param[in] area_variation clustered_idx�ɑΉ�����ʐς̕ω���
\param[in] mser_idx area_variation�ŋɏ��l���Ƃ�C���f�b�N�X
*/
void GetLocalVariationMaxima(const std::vector<std::vector<int> >& clustered_idx,
	const std::vector<std::vector<double> >& area_variation, 
	std::vector<int>& mser_idx)
{
	int num_vec = area_variation.size();

	if(num_vec != clustered_idx.size()){
		throw std::exception("num_vec != clustered_idx.size()");
	}

	for(int i=0; i<num_vec; i++){
		double min = 10000;
		int min_id = -1;
		int num_rgn = area_variation[i].size();
		for(int j=0; j<num_rgn; j++){
			if(area_variation[i][j] >= 0){
				if(min > area_variation[i][j]){
					min = area_variation[i][j];
					min_id = j;
				}
			}
		}
		if(min_id >= 0)
			mser_idx.push_back(clustered_idx[i][min_id]);
	}
/*	for(int i=0; i<num_vec; i++){
		double prev = -1;
		int num_rgn = area_variation[i].size();
		for(int j=0; j<num_rgn; j++){
			if(area_variation[i][j] >= 0){
				if(prev > 0 && prev < area_variation[i][j]){
					mser_idx.push_back(clustered_idx[i][j-1]);
				}
				prev = area_variation[i][j];
			}
		}
	}*/
}


//! �P����Maximally Stable Extreme Regions (MSER)�̒��o
/*!
\param[in] histogram �P�����M��
\param[out] msers ���o���ꂽ�̈�Bfirst: �J�n�ʒu�Asecond�F����
\param[in] step �������l�ω��̃X�e�b�v
\param[in] delta �Ǐ��I�ȋɏ��l�����߂邽�߂̕�
\param[in] min_area ���o�ŏ��T�C�Y
\param[in] max_area ���o�ő�T�C�Y
\param[in] mask �����}�X�N
*/
template<typename _T> 
void Mser1D(const std::vector<_T>& histogram, std::vector<std::pair<int, int> >& msers, 
	double step = 1.0, double delta = 1.0, int min_area = 1, int max_area = -1, 
	const std::vector<unsigned char>& mask = std::vector<unsigned char>())
{
	// �}�X�N��������
	std::vector<_T> masked_histogram;
	Mask1D(histogram, masked_histogram, mask);
	
	// �������ւ������l��ω������Ȃ���A�������l�ȏ�̘A���̈��؍\���Ƃ��Ď擾
	std::vector<REGION_1D> regions;
	CreateMserRegionTree(masked_histogram, regions, 0, step);

	// �؍\���𕪉����ăN���X�^�����O(���m�[�h�����P�ł���A���̈�)
	std::vector<std::vector<int> > clustered_region_indices;
	ClusterRegionsFromTree(regions, clustered_region_indices);

	// �̈�̕ω��ʂ��Z�o
	int i_delta = (int)(delta / step + 0.5);
	if(max_area < 1)
		max_area = histogram.size();
	std::vector<std::vector<double> > area_variation;
	AreaVariation(regions, area_variation, clustered_region_indices, i_delta, min_area, max_area);

	// �̈�̕ω��ʂ̋Ǐ��I�ɍŏ��ȃC���f�b�N�X���Z�o
	std::vector<int> mser_idx;
	GetLocalVariationMaxima(clustered_region_indices, area_variation, mser_idx);

	// Maximally stable extremal regions���擾
	std::vector<int>::iterator it, it_end = mser_idx.end();
	for(it = mser_idx.begin(); it != it_end; it++){
		std::pair<int,int> rgn;
		rgn.first = regions[*it].pos;
		rgn.second = regions[*it].len;
		msers.push_back(rgn);
	}
}

}
#endif