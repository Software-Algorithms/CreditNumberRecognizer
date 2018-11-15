CreditNumberRecognizer

2018/11/14 Version 1.2
- �R�}���h���C�����璼�ڎ��s�ł���@�\�ǉ�
- �J�����L���v�`������g�p����@�\��ǉ�
- CMakeFile.txt�̒ǉ�
- �Ή�OpenCV�̃o�[�W�����A�b�v

2015/04/26 First Version

--------------------
�{�v���O�����͉摜����N���W�b�g�J�[�h�ԍ���ǂݎ��܂��B
�N���W�b�g�J�[�h���ł��邾����ʂ����ς��ɁA�X�����Ȃ��悤�ɎB�e���āA�摜��ۑ����ĉ������B

CreditNumberRecognizer.zip���𓀂���ƁAWindows�ł̎��s�t�@�C�����������p�ł��܂��B�i�vVisualStuido2017�����^�C�����C�u�����j

���A�{�v���O�����͈ȉ��̊��œ���m�F����܂����B
- Windows10 & CentOS7.4
- Visual Stuido2017
- OpenCV 3.4.3
- Boost 1.68

�r���h���@(Linux)�F
���O��OpenCV(https://opencv.org/)��(https://www.boost.org/)���C���X�g�[������Ă�����̂Ƃ��܂��B
$ git clone https://github.com/takmin/CreditNumberRecognizer.git
$ cd CreditNumberRecognizer
$ mkdir build
$ cd build
$ cmake ..
$ make
�����ŁACreditNumberRecognizer�Ƃ������s�t�@�C������������܂��B


�g�p�菇�i�C���^���N�V�������[�h�j�F
1. CreditNumberRecognizer���N��
2. "command:"�Ƃ����v�����v�g���o��̂ŁA"load"�Ƒł����݃��^�[��
3. "Classifier File:"�Ƃ����v�����v�g���o��̂ŁA��������"CreditModel.txt"�ւ̖������w��
4. "command:"�v�����v�g�ŁA"recog"�Ƃ������^�[��
5. "Image File Name:"�v�����v�g�ŁA�N���W�b�g�J�[�h�摜�ւ̃t�@�C���p�X���w��B
6. "Save File Name:"�v�����v�g�ŁA�o�͉摜�����L�q�B���̖��O�Ō��ʂ��ۑ�����܂��B
7. "exit"�Ńv���O�������I��

���̑��F
�E"recog_folder"�Ɠ��͂��A"Directory Name:"�v�����v�g�Ńt�H���_�����w�肷��ƁA�w�肵���t�H���_���ɂ���摜��S�Ĉꊇ�ŏ������Ă���܂��B�o�͐��"Save Directory"�Ŏw�肵���t�H���_�̒��ɕۑ�����܂��B
�E"recog_capture"�Ɠ��͂���ƁA�J�����L���v�`������ʂ������オ��܂��B�g���ɃN���W�b�g�J�[�h�����ĉ����L�[�������ƁA�摜���L���v�`������Ĕԍ����F������܂��B
�E�R�}���h�v�����v�g��"h"�Ƃ��Ǝg�p�\�ȃR�}���h�ꗗ���o�܂��B


�R�}���h���C�����璼�ڎ��s�F
�E�N�����Ɉ������w�肷�邱�ƂŁA�C���^���N�V�����Ȃ��ɁA�R�}���h���C�����璼�ڎ��s�ł��܂��B
---
CreditNumberRecognizer input [option]
option:
  -i [ --input ] arg                    ���͉摜�܂��͉摜�̓������t�H���_�ւ̃p�X
  -h [ --help ]                         �w���v�̕\��
  -m [ --model ] arg (=CreditModel.txt) ���f���t�@�C�����w��
  -o [ --output ] arg                   �F�����ʂ��摜�Ƃ��ĕۑ��Binput���t�H���_�̎��̓t�H���_�ւ̃p�X
  -c [ --camera ]                       Web�J�����̓��͂��g�p
----

���ӁF
�E����A�G���[�����Ȃǂ͑g�ݍ���ł��܂���̂ŁA�C���M�����[�Ȏg�p�@�ŗ����邱�Ƃ�����܂��B
�E�摜�ɂ̓N���W�b�g�J�[�h�S�̂��ʂ��Ă���A�]�v�Ȕw�i���̉f�荞�݂��Ȃ����̂Ƃ��܂��B

Hope to do�F
�E�J�[�h�̌X���␳�̎���
�E�����F���̐��x����i�v�w�K�f�[�^�j


������Ѓr�W������IT���{�@
�F���� (http://visitlab.jp)