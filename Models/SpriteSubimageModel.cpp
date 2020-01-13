#include "SpriteSubimageModel.h"
#include "Components/Logger.h"

#include <QDataStream>
#include <QIcon>
#include <QImageReader>
#include <QMimeData>

SpriteSubimageModel::SpriteSubimageModel(MutableRepeatedFieldRef<std::string> protobuf, const FieldDescriptor* field,
                                         ProtoModelPtr parent)
    : RepeatedStringModel(protobuf, field, parent), _maxIconSize(128, 128), _minIconSize(16, 16) {}

void SpriteSubimageModel::SetMaxIconSize(unsigned width, unsigned height) {
  _maxIconSize = QSize(static_cast<int>(width), static_cast<int>(height));
}

void SpriteSubimageModel::SetMinIconSize(unsigned width, unsigned height) {
  _minIconSize = QSize(static_cast<int>(width), static_cast<int>(height));
}

QSize SpriteSubimageModel::GetIconSize() {
  if (rowCount() > 0)
    return data(index(0), Qt::SizeHintRole).toSize();
  else
    return QSize(32, 32);
}

QVariant SpriteSubimageModel::data(int row) const { return data(index(row), Qt::UserRole); }

QVariant SpriteSubimageModel::data(const QModelIndex& index, int role) const {
  R_EXPECT(index.isValid(), QVariant()) << "Supplied index was invalid:" << index;

  if (role == Qt::DecorationRole)
    return QIcon(QString::fromStdString(strings.Get(index.row())));
  else if (role == Qt::SizeHintRole) {
    // Don't load image we just need size
    QImageReader img(QString::fromStdString(strings.Get(index.row())));
    QSize actualSize = img.size();
    float aspectRatio = static_cast<float>(qMin(actualSize.width(), actualSize.height())) /
                        qMax(actualSize.width(), actualSize.height());

    int width = qMin(actualSize.width(), _maxIconSize.width());
    int height = qMin(actualSize.height(), _maxIconSize.height());

    if (actualSize.width() > _maxIconSize.width() || actualSize.height() > _maxIconSize.height()) {
      if (actualSize.width() < actualSize.height()) width *= aspectRatio;
      if (actualSize.width() > actualSize.height()) height *= aspectRatio;
    }

    return QSize(qMax(_minIconSize.width(), width), qMax(_minIconSize.height(), height));

  } else if (role == Qt::UserRole) {
    return QString::fromStdString(strings.Get(index.row()));
  } else {
    return QVariant();
  }
}

QMimeData* SpriteSubimageModel::mimeData(const QModelIndexList& indexes) const {
  QMimeData* mimeData = RepeatedStringModel::mimeData(indexes);
  mimeData->setProperty("ImageSize", data(index(0), Qt::SizeHintRole));
  return mimeData;
}

bool SpriteSubimageModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                                       const QModelIndex& parent) {
  if (action == Qt::IgnoreAction) return true;
  if (!data->hasFormat(mimeTypeStr)) return false;
  if (column > 0) return false;

  if (rowCount() > 0) {  // if theres existing subimage sizes need to matches
    QSize expectedSize = this->data(index(0), Qt::SizeHintRole).toSize();
    QSize actualSize = data->property("ImageSize").toSize();
    if (expectedSize != actualSize) {
      emit MismatchedImageSize(expectedSize, actualSize);
      return false;
    }
  }  // if empty we'll take whatever size image

  // TODO: Copy the files to the correct EGM folder amd update the filepath
  return RepeatedStringModel::dropMimeData(data, action, row, column, parent);
}
