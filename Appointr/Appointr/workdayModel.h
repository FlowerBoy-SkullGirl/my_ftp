#include "workday.h"
#include <QAbstractTableModel>

#ifndef WORKDAY_MODEL_H
#define WORKDAY_MODEL_H
class WorkdayModel : public QAbstractTableModel{
	Q_OBJECT

	protected:
		Workday work_data;
	public:
		WorkdayModel(QObject * parent = nullptr);
		int rowCount(const QModelIndex &parent = QModelIndex()) const override;
		int columnCount(const QModelIndex &parent = QModelIndex()) const override;
		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
		void set_data(const Workday data);
		
		virtual ~WorkdayModel(){};
};
#endif
