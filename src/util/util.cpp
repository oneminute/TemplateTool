#include "util.h"

bool hImage2QImage(HalconCpp::HImage& from, QImage& to)
{
	Hlong width;
	Hlong height;
	from.GetImageSize(&width, &height);

	HalconCpp::HTuple channels = from.CountChannels();
	HalconCpp::HTuple type = from.GetImageType();

	if (strcmp(type[0].S(), "byte"))
	{
		return false;
	}

	QImage::Format format;
	switch (channels[0].I())
	{
	case 1:
		format = QImage::Format_Grayscale8;
		break;
	case 3:
		format = QImage::Format_RGB32;
		break;
	default:
		return false;
	}

	if (to.width() != width || to.height() != height || to.format() != format)
	{
		to = QImage(static_cast<int>(width),
			static_cast<int>(height),
			format);
	}
	HalconCpp::HString Type;
	if (channels[0].I() == 1)
	{
		unsigned char* pSrc = reinterpret_cast<unsigned char*>(from.GetImagePointer1(&Type, &width, &height));
		memcpy(to.bits(), pSrc, static_cast<size_t>(width) * static_cast<size_t>(height));
		return true;
	}
	else if (channels[0].I() == 3)
	{
		uchar* R, * G, * B;
		from.GetImagePointer3(reinterpret_cast<void**>(&R),
			reinterpret_cast<void**>(&G),
			reinterpret_cast<void**>(&B), &Type, &width, &height);

		for (int row = 0; row < height; row++)
		{
			QRgb* line = reinterpret_cast<QRgb*>(to.scanLine(row));
			for (int col = 0; col < width; col++)
			{
				line[col] = qRgb(*R++, *G++, *B++);
			}
		}
		return true;
	}

	return false;
}
