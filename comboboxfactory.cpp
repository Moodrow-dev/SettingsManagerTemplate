#include "comboboxfactory.h"
#include <QComboBox>

ComboBoxFactory::ComboBoxFactory(const QStringList& options) : options_(options) {}

QWidget* ComboBoxFactory::create() const {
    QComboBox* comboBox = new QComboBox();
    comboBox->addItems(options_);
    return comboBox;
}
