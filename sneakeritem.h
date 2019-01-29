#ifndef SNEAKERITEM_H
#define SNEAKERITEM_H

#include <QDate>
#include <QImage>
#include <QtCore/QDataStream>
//#include <QVariant>

class SneakerItem
{
public:
    SneakerItem();
    SneakerItem(const SneakerItem &otherSneaker);
    SneakerItem(const QString brand, const QString model, const QString colorway, const QString modelnr, const QDate releasedate, const QDate buydate, const double price,
                const QString seller, const double size, const QImage &image1, const QImage &image2);

    bool operator!=(const SneakerItem &sneaker);
    double getPrice() const { return m_price; }
    double getSize() const { return m_size; }
    friend QDataStream & operator<<(QDataStream &out, const SneakerItem &sneaker);
    friend QDataStream & operator>>(QDataStream &in, SneakerItem &sneaker);
    QDate getBuydate() const { return m_buydate; }
    QDate getReleasedate() const { return m_releasedate; }
    QImage getImage1() const { return m_image1; }
    QImage getImage2() const { return m_image2; }
    QString getBrand() const { return m_brand; }
    QString getColorway() const { return m_colorway; }
    QString getModel() const { return m_model; }
    QString getModelnr() const { return m_modelnr; }
    QString getSeller() const { return m_seller; }
    void setData(const SneakerItem &otherSneaker);
    void setData(const QString brand, const QString model, const QString colorway, const QString modelnr, const QDate releasedate, const QDate buydate, const double price,
                 const QString seller, const double size, const QImage &image1, const QImage &image2);

protected:
    QString m_brand;
    QString m_model;
    QString m_colorway;
    QString m_modelnr;
    QDate m_releasedate;
    QDate m_buydate;
    double m_price;
    QString m_seller;
    double m_size;
    QImage m_image1;
    QImage m_image2;
};

Q_DECLARE_METATYPE(SneakerItem);

#endif // SNEAKERITEM_H
