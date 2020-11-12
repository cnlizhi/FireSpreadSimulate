//CImgProcess.cpp

#include "CImgProcess.h"

#define _MAIN_FILTER 1,1,1,1,1,1,1,1,1
#define _GAUSS_FILTER 1,4,7,4,1,4,16,26,16,4,7,26,41,26,7,4,16,26,16,4,1,4,7,4,1
#define _SHARPNESS_FILTER -1,-1,-1,-1,9,-1,-1,-1,-1
#define _EDGE_DETECTION -1,-1,-1,-1,8,-1,-1,-1,-1
#define _EMBOSSING_FILTER -1,-1,0,-1,0,1,0,1,1

CImgProcess::CImgProcess()
    : m_pppDN(NULL), m_Red(0), m_Grn(0), m_Blu(0),
      m_columns(0), m_rows(0), m_bands(0), m_angle(0), m_zoom(1)
{
    m_ppBoard = new bool*[m_rows + 2];
    for (int i = 0; i < m_rows + 2; i++)
    {
        m_ppBoard[i] = new bool[m_columns + 2];
    }
    for (int i = 0; i < m_columns + 2; i++)
    {
        m_ppBoard[0][i] = false;
        m_ppBoard[m_rows + 1][i] = false;
    }
    for (int i = 0; i < m_rows; i++)
    {
        m_ppBoard[i + 1][0] = false;
        m_ppBoard[i + 1][m_columns + 1] = false;
        for (int j = 0; j < m_columns; j++)
        {
            m_ppBoard[i + 1][j + 1] = false;
        }
    }
}

CImgProcess::CImgProcess(const CImgProcess& rIProcess)
    : m_pppDN(rIProcess.m_pppDN), m_Red(rIProcess.m_Red),
      m_Grn(rIProcess.m_Grn), m_Blu(rIProcess.m_Blu),
      m_columns(rIProcess.m_columns),
      m_rows(rIProcess.m_rows), m_bands(rIProcess.m_bands),
      m_angle(rIProcess.m_angle), m_zoom(rIProcess.m_zoom)
{
    m_ppBoard = new bool*[m_rows + 2];
    for (int i = 0; i < m_rows + 2; i++)
    {
        m_ppBoard[i] = new bool[m_columns + 2];
    }
    for (int i = 0; i < m_columns + 2; i++)
    {
        m_ppBoard[0][i] = false;
        m_ppBoard[m_rows + 1][i] = false;
    }
    for (int i = 0; i < m_rows; i++)
    {
        m_ppBoard[i + 1][0] = false;
        m_ppBoard[i + 1][m_columns + 1] = false;
        for (int j = 0; j < m_columns; j++)
        {
            if ((m_pppDN[2][i][j] / 3 + m_pppDN[1][i][j] / 3 + m_pppDN[0][i][j] / 3) < 20)
            {
                m_ppBoard[i + 1][j + 1] = true;
            }
            else
            {
                m_ppBoard[i + 1][j + 1] = false;
            }
        }
    }
}

CImgProcess::~CImgProcess()
{
    if (m_pppDN != NULL)
    {
        delete[] m_pppDN;
    }
    delete[] m_ppBoard;
}

bool CImgProcess::Set(CRSImage& RSImage)
{
    m_bands = RSImage.GetBands();
    m_rows = RSImage.GetLines();
    m_columns = RSImage.GetSamples();
    m_pppDN = new DN**[m_bands];
    if (m_pppDN == NULL)
    {
        return false;
    }
    for (int i = 0; i < m_bands; ++i)
    {
        m_pppDN[i] = 0UL;
        m_pppDN[i] = new DN*[m_rows];
        if (m_pppDN[i] == NULL)
        {
            return false;
        }
        for (int j = 0; j < m_rows; ++j)
        {
            m_pppDN[i][j] = 0UL;
            m_pppDN[i][j] = new DN[m_columns];
        }
    }
    for (int i = 0; i < m_bands; i++)
    {
        for (int j = 0; j < m_rows; j++)
        {
            for (int k = 0; k < m_columns; k++)
            {
                m_pppDN[i][j][k] = RSImage.GetDN()[i][j][k];
            }
        }
    }
    if(m_bands > 2)
    {
        m_Blu = 0;
        m_Grn = 1;
        m_Red = 2;
    }
    else
    {
        m_Blu = 0;
        m_Grn = 0;
        m_Red = 0;
    }
    m_ppBoard = new bool*[m_rows + 2];
    for (int i = 0; i < m_rows + 2; i++)
    {
        m_ppBoard[i] = new bool[m_columns + 2];
    }
    for (int i = 0; i < m_columns + 2; i++)
    {
        m_ppBoard[0][i] = false;
        m_ppBoard[m_rows + 1][i] = false;
    }
    for (int i = 0; i < m_rows; i++)
    {
        m_ppBoard[i + 1][0] = false;
        m_ppBoard[i + 1][m_columns + 1] = false;
        for (int j = 0; j < m_columns; j++)
        {
            if ((m_pppDN[m_Red][i][j] / 3 + m_pppDN[m_Blu][i][j] / 3 + m_pppDN[m_Grn][i][j] / 3) < 20)
            {
                m_ppBoard[i + 1][j + 1] = true;
            }
            else
            {
                m_ppBoard[i + 1][j + 1] = false;
            }
        }
    }
    return true;
}

void CImgProcess::ReSet()
{
    m_pppDN = NULL;
    m_Red = 0;
    m_Grn = 0;
    m_Blu = 0;
    m_bands = 0;
    m_rows = 0;
    m_columns = 0;
    m_angle = 0;
    m_ppBoard = NULL;
    return;
}

QImage CImgProcess::Display()
{
    DN** Col[3] = { m_pppDN[m_Red],m_pppDN[m_Grn],m_pppDN[m_Blu] };
    DN* pBuffer = new DN[3*m_rows*m_columns];
    int i = 0;
    for (int k = 0; k< m_rows; k++)
    {
        for (int l = 0; l < m_columns; l++)
        {
            for (int j = 0; j < 3 ; j++)
            {
                pBuffer[i] = Col[j][k][l];
                i++;
            }
        }
    }
    QImage Image(pBuffer, m_columns, m_rows, QImage::Format_RGB888);
    QMatrix LeftMatrix;
    LeftMatrix.rotate(m_angle);
    Image = Image.transformed(LeftMatrix, Qt::SmoothTransformation);
    QSize picSize(Image.width()*m_zoom, Image.height()*m_zoom);
    Image = Image.scaled(picSize, Qt::KeepAspectRatio);
    return Image;
}

QImage CImgProcess::Display(DN*** pppDN)
{
    DN** Col[3] = { pppDN[m_Red],pppDN[m_Grn],pppDN[m_Blu] };
    int Maximum[3] = { GetMaximum(Col[0]),GetMaximum(Col[1]),GetMaximum(Col[2]) };
    int Minimum[3] = { GetMinimum(Col[0]),GetMinimum(Col[1]),GetMinimum(Col[2]) };
    DN* pBuffer = new DN[3*m_rows*m_columns];
    int i = 0;
    for (int k = 0; k< m_rows; k++)
    {
        for (int l = 0; l < m_columns; l++)
        {
            for (int j = 0; j < 3 ; j++)
            {
                pBuffer[i] = (Col[j][k][l] - Minimum[j]) * 255 / (Maximum[j] - Minimum[j]);
                i++;
            }
        }
    }
    QImage Image(pBuffer, m_columns, m_rows, QImage::Format_RGB888);
    QMatrix LeftMatrix;
    LeftMatrix.rotate(m_angle);
    Image = Image.transformed(LeftMatrix, Qt::SmoothTransformation);
    QSize picSize(Image.width()*m_zoom, Image.height()*m_zoom);
    Image = Image.scaled(picSize, Qt::KeepAspectRatio);
    return Image;
}

void CImgProcess::SetChannel(int Red, int Grn, int Blu)
{
    m_Red = Red;
    m_Grn = Grn;
    m_Blu = Blu;
    return;
}

void CImgProcess::SetAngle(double angle, bool bFlag)
{
    if(!bFlag)
    {
        m_angle = angle;
        return;
    }
    m_angle += angle;
    return;
}

void CImgProcess::SetZoom(double zoom, bool bFlag)
{
    if(!bFlag)
    {
        m_zoom = zoom;
        return;
    }
    m_zoom *= zoom;
    return;
}

QImage CImgProcess::Filter(char core)
{
    int size = 0;
    if (core == 'M' || core == 'S' || core == 'D' || core == 'E')
    {
        size = 3;
    }
    else if (core == 'G')
    {
        size = 5;
    }
    else
    {
        return Display();
    }
    if (size % 2 == 0)
    {
        return Display();
    }
    double* CoreTemp = NULL;
    switch (core)
    {
    case 'M':
    {
        double BuffCore[9] = { _MAIN_FILTER };
        CoreTemp = BuffCore;
        break;
    }
    case 'G':
    {
        double BuffCore[25] = { _GAUSS_FILTER };
        CoreTemp = BuffCore;
        break;
    }
    case 'S':
    {
        double BuffCore[9] = { _SHARPNESS_FILTER };
        CoreTemp = BuffCore;
        break;
    }
    case 'D':
    {
        double BuffCore[9] = { _EDGE_DETECTION };
        CoreTemp = BuffCore;
        break;
    }
    case 'E':
    {
        double BuffCore[9] = { _EMBOSSING_FILTER };
        CoreTemp = BuffCore;
        break;
    }
    default:
        return Display();
        break;
    }
    DN*** pppDNBuff = new DN**[m_bands];
    if (pppDNBuff == NULL)
    {
        return Display();
    }
    for (int i = 0; i < m_bands; ++i)
    {
        pppDNBuff[i] = 0UL;
        pppDNBuff[i] = new DN*[m_rows];
        if (pppDNBuff[i] == NULL)
        {
            return Display();
        }
        for (int j = 0; j < m_rows; ++j)
        {
            pppDNBuff[i][j] = 0UL;
            pppDNBuff[i][j] = new DN[m_columns];
        }
    }
    for (int i = 0; i < m_bands; i++)
    {
        for (int j = 0; j < m_rows; j++)
        {
            for (int k = 0; k < m_columns; k++)
            {
                pppDNBuff[i][j][k] = m_pppDN[i][j][k];
            }
        }
    }
    double total = 0;
    for (int i = 0; i < size*size; i++)
    {
        total += CoreTemp[i];
    }
    for (int i = 0; i < m_bands; i++)
    {
        for (int j = size / 2; j < m_rows - size / 2; j++)
        {
            for (int k = size / 2; k < m_columns - size / 2; k++)
            {
                double sum = 0;
                int index = 0;
                for (int l = j - size / 2; l < j + size / 2 + 1; l++)
                {
                    for (int m = k - size / 2; m < k + size / 2 + 1; m++)
                    {
                        sum += m_pppDN[i][l][m] * CoreTemp[index++];
                    }
                }
                if (total != 0)
                {
                    sum /= total;
                }
                if (sum > 255)
                {
                    sum = 255;
                }
                if (sum < 0)
                {
                    sum = 0;
                }
                pppDNBuff[i][j][k] = static_cast<DN>(sum);
            }

        }
    }
    return Display(pppDNBuff);
}

int CImgProcess::GetMaximum(DN** band) const
{
    int result = 0;
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            result = (band[i][j] > result) ? band[i][j] : result;
        }
    }
    return result;
}

int CImgProcess::GetMinimum(DN** band) const
{
    int result = 256;
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            result = (band[i][j] < result) ? band[i][j] : result;
        }
    }
    return result;
}

void CImgProcess::SetFire(int Loc_x, int Loc_y)
{
    if(Loc_x > m_columns || Loc_y > m_rows)
    {
        return;
    }
    if (m_ppBoard[Loc_y + 1][Loc_x + 1])
    {
        m_pppDN[m_Red][Loc_y][Loc_x] = 255;
        m_pppDN[m_Grn][Loc_y][Loc_x] = 0;
        m_pppDN[m_Blu][Loc_y][Loc_x] = 0;
        m_ppBoard[Loc_y + 1][Loc_x + 1] = false;
        SetFire(Loc_x - 1, Loc_y - 1);
        SetFire(Loc_x - 1, Loc_y);
        SetFire(Loc_x - 1, Loc_y + 1);
        SetFire(Loc_x, Loc_y - 1);
        SetFire(Loc_x, Loc_y + 1);
        SetFire(Loc_x + 1, Loc_y - 1);
        SetFire(Loc_x + 1, Loc_y);
        SetFire(Loc_x + 1, Loc_y + 1);
        return;
    }
    else
    {
        return;
    }
}

int CImgProcess::GetArea()
{
    int count = 0;
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            if (m_ppBoard[i][j])
            {
                count++;
            }
        }
    }
    return count;
}
