// ���s�����̐�
static const int DIRLIGHT_NUM = 3;
struct DirLight
{
    float3 lightv; // ���C�g�ւ̕����̒P�ʃx�N�g��
    float3 lightcolor; // ���C�g�̐F(RGB)
};

//�_����
static const int POINTLIGHT_NUM = 9;
struct PointLight
{
    float3 lightpos; //���C�g���W
    float3 lightcolor; //���C�g�̐F
    float3 lightatten; //���C�g�̋��������W��
    uint active;
};

//�X�|�b�g���C�g
static const int SPOTLIGHT_NUM = 3;
struct SpotLight
{
    float3 lightv; // ���C�g�ւ̕����̒P�ʃx�N�g��
    float3 lightpos; //���C�g���W
    float3 lightcolor; //���C�g�̐F
    float3 lightatten; //���C�g�̋��������W��
    float2 lightfactoranglecos; //���C�g�����p�x�̃R�T�C��
    uint active;
};

//�ۉe�̐�
static const int CIRCLESHADOW_NUM = 1;

struct CircleShadow
{
    float3 dir; // ���e�����̋t�x�N�g��(�P�ʃx�N�g��)
    float3 casterPos; //�L���X�^�[���W
    float distanceCasterLight; //�L���X�^�[�ƃ��C�g�̋���
    float3 atten; //���������W��
    float2 factorAngleCos; //�����p�x�̃R�T�C��
    uint active;

};
