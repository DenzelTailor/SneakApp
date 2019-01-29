#include "sneakeritem.h"

SneakerItem::SneakerItem()
{
    m_brand = "Brand";
    m_model = "Model";
    m_colorway = "Colorway";
    m_modelnr = "Model Nr.";
    m_releasedate = QDate::currentDate();
    m_buydate = QDate::currentDate();
    m_price = 0.;
    m_seller = "Seller";
    m_size = 10.5;
    m_image1 = QImage(":/Images/question.png");
    m_image2 = QImage(":/Images/question.png");
}

SneakerItem::SneakerItem(const SneakerItem &otherSneaker)
{
    m_brand = otherSneaker.m_brand;
    m_model = otherSneaker.m_model;
    m_colorway = otherSneaker.m_colorway;
    m_modelnr = otherSneaker.m_modelnr;
    m_releasedate = otherSneaker.m_releasedate;
    m_buydate = otherSneaker.m_buydate;
    m_price = otherSneaker.m_price;
    m_seller = otherSneaker.m_seller;
    m_size = otherSneaker.m_size;
    m_image1 = otherSneaker.m_image1;
    m_image2 = otherSneaker.m_image2;
}

SneakerItem::SneakerItem(const QString brand, const QString model, const QString colorway, const QString modelnr, const QDate releasedate, const QDate buydate, const double price,
                         const QString seller, const double size, const QImage &image1, const QImage &image2)
{
    m_brand = brand;
    m_model = model;
    m_colorway = colorway;
    m_modelnr = modelnr;
    m_releasedate = releasedate;
    m_buydate = buydate;
    m_price = price;
    m_seller = seller;
    m_size = size;

    if (image1.isNull())
    {
        m_image1 = QImage(":/Images/question.png");
    }
    else
    {
        m_image1 = image1;
    }

    if (image2.isNull())
    {
        m_image2 = QImage(":/Images/question.png");
    }
    else
    {
        m_image2 = image2;
    }
}

bool SneakerItem::operator!=(const SneakerItem &sneaker)
{
    return this->m_brand != sneaker.m_brand &&
            this->m_model != sneaker.m_model &&
            this->m_colorway != sneaker.m_colorway &&
            this->m_modelnr != sneaker.m_modelnr &&
            this->m_releasedate != sneaker.m_releasedate &&
            this->m_buydate != sneaker.m_buydate &&
            (fabs(this->m_price - sneaker.m_price) >= 0.009) &&
            this->m_seller != sneaker.m_seller &&
            (fabs(this->m_size - sneaker.m_size) >= 0.4) &&
            this->m_image1 != sneaker.m_image1 &&
            this->m_image2 != sneaker.m_image2;
}

QDataStream & operator<<(QDataStream &out, const SneakerItem &sneaker)
{
    out << sneaker.m_brand;
    out << sneaker.m_model;
    out << sneaker.m_colorway;
    out << sneaker.m_modelnr;
    out << sneaker.m_releasedate;
    out << sneaker.m_buydate;
    out << sneaker.m_price;
    out << sneaker.m_seller;
    out << sneaker.m_size;
    out << sneaker.m_image1;
    out << sneaker.m_image2;
    return out;
}

QDataStream & operator>>(QDataStream &in, SneakerItem &sneaker)
{
    in >> sneaker.m_brand;
    in >> sneaker.m_model;
    in >> sneaker.m_colorway;
    in >> sneaker.m_modelnr;
    in >> sneaker.m_releasedate;
    in >> sneaker.m_buydate;
    in >> sneaker.m_price;
    in >> sneaker.m_seller;
    in >> sneaker.m_size;
    in >> sneaker.m_image1;
    in >> sneaker.m_image2;
    return in;
}

void SneakerItem::setData(const SneakerItem &otherSneaker)
{
    m_brand = otherSneaker.m_brand;
    m_model = otherSneaker.m_model;
    m_colorway = otherSneaker.m_colorway;
    m_modelnr = otherSneaker.m_modelnr;
    m_releasedate = otherSneaker.m_releasedate;
    m_buydate = otherSneaker.m_buydate;
    m_price = otherSneaker.m_price;
    m_seller = otherSneaker.m_seller;
    m_size = otherSneaker.m_size;
    m_image1 = otherSneaker.m_image1;
    m_image2 = otherSneaker.m_image2;
}

void SneakerItem::setData(QString brand, const QString model, const QString colorway, const QString modelnr, const QDate releasedate, const QDate buydate, const double price,
                         const QString seller, const double size, const QImage &image1, const QImage &image2)
{
    m_brand = brand;
    m_model = model;
    m_colorway = colorway;
    m_modelnr = modelnr;
    m_releasedate = releasedate;
    m_buydate = buydate;
    m_price = price;
    m_seller = seller;
    m_size = size;

    if (image1.isNull())
    {
        m_image1 = QImage(":/Images/question.png");
    }
    else
    {
        m_image1 = image1;
    }

    if (image2.isNull())
    {
        m_image2 = QImage(":/Images/question.png");
    }
    else
    {
        m_image2 = image2;
    }
}
