/****************************************************************************/
//    copyright 2012 - 2016  Chris Rizzitello <sithlord48@gmail.com>        //
//                                                                          //
//    This file is part of FF7tk                                            //
//                                                                          //
//    FF7tk is free software: you can redistribute it and/or modify         //
//    it under the terms of the GNU General Public License as published by  //
//    the Free Software Foundation, either version 3 of the License, or     //
//    (at your option) any later version.                                   //
//                                                                          //
//    FF7tk is distributed in the hope that it will be useful,              //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of        //
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU General Public License for more details.                          //
/****************************************************************************/
#include "CharEditor.h"
//#include <QDebug>
CharEditor::CharEditor(qreal Scale,QWidget *parent) : QWidget(parent)
  , load(false)
  , autolevel(true)
  , autostatcalc(true)
  , editable(true)
  , advancedMode(false)
  , mslotsel(-1)
  , scale(Scale)
{
	init_display();
	init_connections();
	//auto level and auto stat calc are enabled by default.
	//always check them when doing these actions.
}

void CharEditor::init_display()
{

    lbl_avatar = new QLabel;
    lbl_avatar->setFixedSize(int(86*scale),int(98*scale));
	line_name = new QLineEdit;
	line_name->setPlaceholderText(tr("Name"));
	lbl_hp = new QLabel(tr("HP"));
	lbl_mp = new QLabel(tr("MP"));
	lbl_mp_slash = new QLabel("/");
	lbl_hp_slash = new QLabel("/");
	lbl_hp_max = new QLabel("");
	lbl_mp_max = new QLabel("");
	sb_level = new QSpinBox;
	sb_level->setPrefix(tr("Level").append(": "));
	sb_curMp = new QSpinBox;
	sb_curHp = new QSpinBox;
	sb_maxMp = new QSpinBox;
	sb_maxMp->setToolTip(tr("value calculated ingame; edit BaseMp"));
	sb_maxMp->setVisible(false);
	sb_maxHp = new QSpinBox;
	sb_maxHp->setToolTip(tr("value calculated ingame; edit BaseHp"));
	sb_maxHp->setVisible(false);

	sb_kills = new QSpinBox;
	sb_kills->setPrefix(tr("Kills").append(": "));
	cb_fury=new QCheckBox(tr("Fury"));
	cb_sadness = new QCheckBox(tr("Sadness"));
	cb_front_row = new QCheckBox(tr("Front Row"));
	cb_idChanger = new QCheckBox;
	cb_idChanger->setHidden(true);

	combo_id = new QComboBox;
	for(int i=0;i<11;i++){combo_id->addItem(Chars.icon(i),Chars.defaultName(i));}
	combo_id->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
	combo_id->setHidden(true);

	lbl_base_hp = new QLabel(tr("Base HP"));
	lbl_base_hp_bonus = new QLabel;
	sb_base_hp = new QSpinBox;
    sb_base_hp->setFixedWidth(fontMetrics().width(QChar('W'))*5);
	lbl_base_mp = new QLabel(tr("Base MP"));
	lbl_base_mp_bonus = new QLabel;
	sb_base_mp = new QSpinBox;
    sb_base_mp->setFixedWidth(fontMetrics().width(QChar('W'))*5);

	lbl_level_progress = new QLabel(tr("Level Progress"));
	bar_tnl = new QProgressBar;
	bar_tnl->setMaximum(61);//strange indeed..
	bar_tnl->setTextVisible(false);
    bar_tnl->setFixedHeight(int(10*scale));
    bar_tnl->setFixedWidth(int(61*scale));
	lbl_level_next = new QLabel(tr("Next"));
	sb_total_exp = new QSpinBox;
	sb_total_exp->setPrefix(tr("Exp: "));
	sb_total_exp->setMaximumWidth(fontMetrics().width(QChar('9'))*16);
	sb_total_exp->setMaximum(2147483647);
	sb_total_exp->setWrapping(true);

	lbl_limit_bar = new QLabel (tr("Limit Bar"));
	slider_limit = new QSlider;
    slider_limit->setMaximumHeight(int(20*scale));
	slider_limit->setMaximum(255);
	slider_limit->setOrientation(Qt::Horizontal);
	lcd_limit_value = new QLCDNumber;
	lcd_limit_value->setSegmentStyle(QLCDNumber::Flat);

	lbl_str = new QLabel(tr("Str"));
	sb_str = new QSpinBox;
	sb_str->setAlignment(Qt::AlignCenter);
	QLabel *lbl_str_plus_1 = new QLabel(tr("+"));
	lbl_str_plus_1->setAlignment(Qt::AlignCenter);
	sb_str_bonus = new QSpinBox;
	sb_str_bonus->setAlignment(Qt::AlignCenter);
	QLabel *lbl_str_plus_2 = new QLabel(tr("+"));
	lbl_str_plus_2->setAlignment(Qt::AlignCenter);
	lbl_str_mat_bonus = new QLabel;
	QLabel *lbl_str_equals = new QLabel(tr("="));
	lbl_str_total = new QLabel;

	lbl_vit = new QLabel(tr("Vit"));
	sb_vit = new QSpinBox;
	sb_vit->setAlignment(Qt::AlignCenter);
	QLabel *lbl_vit_plus_1 = new QLabel(tr("+"));
	lbl_vit_plus_1->setAlignment(Qt::AlignCenter);
	sb_vit_bonus = new QSpinBox;
	sb_vit_bonus->setAlignment(Qt::AlignCenter);
	QLabel *lbl_vit_plus_2 = new QLabel(tr("+"));
	lbl_vit_plus_2->setAlignment(Qt::AlignCenter);
	lbl_vit_mat_bonus = new QLabel;
	QLabel *lbl_vit_equals = new QLabel(tr("="));
	lbl_vit_total = new QLabel;

	lbl_mag = new QLabel(tr("Mag"));
	sb_mag = new QSpinBox;
	sb_mag->setAlignment(Qt::AlignCenter);
	QLabel *lbl_mag_plus_1 = new QLabel(tr("+"));
	lbl_mag_plus_1->setAlignment(Qt::AlignCenter);
	sb_mag_bonus = new QSpinBox;
	sb_mag_bonus->setAlignment(Qt::AlignCenter);
	QLabel *lbl_mag_plus_2 = new QLabel(tr("+"));
	lbl_mag_plus_2->setAlignment(Qt::AlignCenter);
	lbl_mag_mat_bonus = new QLabel;
	QLabel *lbl_mag_equals = new QLabel(tr("="));
	lbl_mag_total = new QLabel;

	lbl_spi = new QLabel(tr("Spi"));
	sb_spi = new QSpinBox;
	sb_spi->setAlignment(Qt::AlignCenter);
	QLabel *lbl_spi_plus_1 = new QLabel(tr("+"));
	lbl_spi_plus_1->setAlignment(Qt::AlignCenter);
	sb_spi_bonus = new QSpinBox;
	sb_spi_bonus->setAlignment(Qt::AlignCenter);
	QLabel *lbl_spi_plus_2 = new QLabel(tr("+"));
	lbl_spi_plus_2->setAlignment(Qt::AlignCenter);
	lbl_spi_mat_bonus = new QLabel;
	QLabel *lbl_spi_equals = new QLabel(tr("="));
	lbl_spi_total = new QLabel;

	lbl_dex = new QLabel(tr("Dex"));
	sb_dex = new QSpinBox;
	sb_dex->setAlignment(Qt::AlignCenter);
	QLabel *lbl_dex_plus_1 = new QLabel(tr("+"));
	lbl_dex_plus_1->setAlignment(Qt::AlignCenter);
	sb_dex_bonus = new QSpinBox;
	sb_dex_bonus->setAlignment(Qt::AlignCenter);
	QLabel *lbl_dex_plus_2 = new QLabel(tr("+"));
	lbl_dex_plus_2->setAlignment(Qt::AlignCenter);
	lbl_dex_mat_bonus = new QLabel;
	QLabel *lbl_dex_equals = new QLabel(tr("="));
	lbl_dex_total = new QLabel;

	lbl_lck = new QLabel(tr("Lck"));
	sb_lck = new QSpinBox;
	sb_lck->setAlignment(Qt::AlignCenter);
	QLabel *lbl_lck_plus_1 = new QLabel(tr("+"));
	lbl_lck_plus_1->setAlignment(Qt::AlignCenter);
	sb_lck_bonus = new QSpinBox;
	sb_lck_bonus->setAlignment(Qt::AlignCenter);
	QLabel *lbl_lck_plus_2 = new QLabel(tr("+"));
	lbl_lck_plus_2->setAlignment(Qt::AlignCenter);
	lbl_lck_mat_bonus = new QLabel;
	QLabel *lbl_lck_equals = new QLabel(tr("="));
	lbl_lck_total = new QLabel;

	//Spin Boxes for limit uses.
	sb_uses_limit_1_1 = new QSpinBox;
	sb_uses_limit_2_1 = new QSpinBox;
	sb_uses_limit_3_1 = new QSpinBox;
    sb_uses_limit_1_1->setFixedWidth(fontMetrics().width(QChar('W'))*5);
    sb_uses_limit_2_1->setFixedWidth(fontMetrics().width(QChar('W'))*5);
    sb_uses_limit_3_1->setFixedWidth(fontMetrics().width(QChar('W'))*5);
	sb_uses_limit_1_1->setAlignment(Qt::AlignCenter);
	sb_uses_limit_2_1->setAlignment(Qt::AlignCenter);
	sb_uses_limit_3_1->setAlignment(Qt::AlignCenter);


	lbl_uses = new QLabel(tr("Limit Uses"));
	lbl_uses->setAlignment(Qt::AlignHCenter);
	lbl_1_1 = new QLabel(tr("1-1"));
	lbl_2_1 = new QLabel(tr("2-1"));
	lbl_3_1 = new QLabel(tr("3-1"));
    lbl_1_1->setFixedWidth(fontMetrics().width(QChar('W'))*3);
    lbl_2_1->setFixedWidth(fontMetrics().width(QChar('W'))*3);
    lbl_3_1->setFixedWidth(fontMetrics().width(QChar('W'))*3);
	lbl_1_1->setAlignment(Qt::AlignHCenter);
	lbl_2_1->setAlignment(Qt::AlignHCenter);
	lbl_3_1->setAlignment(Qt::AlignHCenter);


	QHBoxLayout *layout_1_1 = new QHBoxLayout;
	layout_1_1->setContentsMargins(0,0,0,0);
	//layout_1_1->setSpacing(3);
	layout_1_1->addWidget(lbl_1_1);
	layout_1_1->addWidget(sb_uses_limit_1_1);

	QHBoxLayout *layout_2_1 = new QHBoxLayout;
	layout_2_1->setContentsMargins(0,0,0,0);
	//layout_2_1->setSpacing(3);
	layout_2_1->addWidget(lbl_2_1);
	layout_2_1->addWidget(sb_uses_limit_2_1);
	QHBoxLayout *layout_3_1 = new QHBoxLayout;
	layout_3_1->setContentsMargins(0,0,0,0);
	//layout_3_1->setSpacing(3);
	layout_3_1->addWidget(lbl_3_1);
	layout_3_1->addWidget(sb_uses_limit_3_1);

	QVBoxLayout *used_limits_layout = new QVBoxLayout;
	used_limits_layout->setContentsMargins(0,0,0,0);
	//used_limits_layout->setSpacing(3);
	used_limits_layout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::MinimumExpanding));
	used_limits_layout->addWidget(lbl_uses);
	used_limits_layout->addLayout(layout_1_1);
	used_limits_layout->addLayout(layout_2_1);
	used_limits_layout->addLayout(layout_3_1);
	used_limits_layout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::MinimumExpanding));

	list_limits = new QListWidget;
	lbl_limit_level = new QLabel(tr("Limit Level"));
	sb_limit_level = new QSpinBox;

    sb_limit_level->setFixedWidth(fontMetrics().width(QChar('W'))*4);

	QHBoxLayout *limit_level_layout = new QHBoxLayout;
	limit_level_layout->setContentsMargins(0,0,0,0);
	limit_level_layout->addWidget(lbl_limit_level);
	limit_level_layout->addWidget(sb_limit_level);

	sb_level->setMaximum(99);
	sb_curMp->setMaximum(32767);
	sb_curHp->setMaximum(32767);
	sb_maxMp->setMaximum(32767);
	sb_maxHp->setMaximum(32767);
	sb_base_hp->setMaximum(32767);
	sb_base_mp->setMaximum(32767);
	sb_kills->setMaximum(65535);
	sb_str->setMaximum(255);
	sb_str_bonus->setMaximum(255);
	sb_vit->setMaximum(255);
	sb_vit_bonus->setMaximum(255);
	sb_mag->setMaximum(255);
	sb_mag_bonus->setMaximum(255);
	sb_spi->setMaximum(255);
	sb_spi_bonus->setMaximum(255);
	sb_dex->setMaximum(255);
	sb_dex_bonus->setMaximum(255);
	sb_lck->setMaximum(255);
	sb_lck_bonus->setMaximum(255);
	sb_limit_level->setMaximum(4);
	sb_uses_limit_1_1->setMaximum(32767);
	sb_uses_limit_2_1->setMaximum(32767);
	sb_uses_limit_3_1->setMaximum(32767);

	sb_curMp->setWrapping(true);
	sb_curHp->setWrapping(true);
	sb_base_hp->setWrapping(true);
	sb_base_mp->setWrapping(true);
	sb_kills->setWrapping(true);
	sb_str->setWrapping(true);
	sb_str_bonus->setWrapping(true);
	sb_vit->setWrapping(true);
	sb_vit_bonus->setWrapping(true);
	sb_mag->setWrapping(true);
	sb_mag_bonus->setWrapping(true);
	sb_spi->setWrapping(true);
	sb_spi_bonus->setWrapping(true);
	sb_dex->setWrapping(true);
	sb_dex_bonus->setWrapping(true);
	sb_lck->setWrapping(true);
	sb_lck_bonus->setWrapping(true);
	sb_limit_level->setWrapping(true);
	sb_uses_limit_1_1->setWrapping(true);
	sb_uses_limit_2_1->setWrapping(true);
	sb_uses_limit_3_1->setWrapping(true);

	line_name->setMaxLength(11);
    line_name->setMaximumWidth(fontMetrics().width(QChar('W'))*11);

	QHBoxLayout * name_level_layout= new QHBoxLayout;
	name_level_layout->addWidget(line_name);
	QSpacerItem *name_spacer = new QSpacerItem(20,0,QSizePolicy::Fixed,QSizePolicy::Fixed);
	name_level_layout->addSpacerItem(name_spacer);
	name_level_layout->addWidget(sb_level);

	QHBoxLayout *hp_layout = new QHBoxLayout;
	hp_layout->addWidget(lbl_hp);
	hp_layout->addWidget(sb_curHp);
	hp_layout->addWidget(lbl_hp_slash);
	hp_layout->addWidget(lbl_hp_max);
	hp_layout->addWidget(sb_maxHp);
	QSpacerItem *hp_spacer1 = new QSpacerItem(20,0,QSizePolicy::Expanding,QSizePolicy::Fixed);
	hp_layout->addSpacerItem(hp_spacer1);
	QSpacerItem *hp_spacer = new QSpacerItem(20,0,QSizePolicy::Expanding,QSizePolicy::Fixed);
	hp_layout->addSpacerItem(hp_spacer);

	QHBoxLayout *mp_layout = new QHBoxLayout;
	mp_layout->addWidget(lbl_mp);
	mp_layout->addWidget(sb_curMp);
	mp_layout->addWidget(lbl_mp_slash);
	mp_layout->addWidget(lbl_mp_max);
	mp_layout->addWidget(sb_maxMp);
	QSpacerItem *mp_spacer1 = new QSpacerItem(20,0,QSizePolicy::Expanding,QSizePolicy::Fixed);
	mp_layout->addSpacerItem(mp_spacer1);
	QSpacerItem *mp_spacer = new QSpacerItem(20,0,QSizePolicy::Expanding,QSizePolicy::Fixed);
	mp_layout->addSpacerItem(mp_spacer);

	QHBoxLayout *barNextLayout = new QHBoxLayout();
	barNextLayout->setContentsMargins(0,0,0,0);
	barNextLayout->addWidget(bar_tnl);
	barNextLayout->addWidget(lbl_level_next);
	barNextLayout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));

	QVBoxLayout *exp_layout = new QVBoxLayout;
	exp_layout->setContentsMargins(0,0,0,0);
	//exp_layout->setSpacing(0);
	exp_layout->addWidget(sb_total_exp);
	exp_layout->addLayout(barNextLayout);

	QHBoxLayout *kills_layout = new QHBoxLayout;
	kills_layout->setContentsMargins(0,0,0,0);
	kills_layout->addWidget(sb_kills);
	kills_layout->addLayout(exp_layout);

	QVBoxLayout *name_hp_mp_kills_layout = new QVBoxLayout;
	//name_hp_mp_kills_layout->setSpacing(0);
	name_hp_mp_kills_layout->addLayout(name_level_layout);
	name_hp_mp_kills_layout->addLayout(hp_layout);
	name_hp_mp_kills_layout->addLayout(mp_layout);
	name_hp_mp_kills_layout->addLayout(kills_layout);

	QVBoxLayout *fury_sadness_layout = new QVBoxLayout;
	fury_sadness_layout->setContentsMargins(0,0,0,0);
	//fury_sadness_layout->setSpacing(2);
	fury_sadness_layout->addWidget(cb_fury);
	fury_sadness_layout->addWidget(cb_sadness);


	QVBoxLayout *sadness_row_id_layout =new QVBoxLayout;
	sadness_row_id_layout->addLayout(fury_sadness_layout);
	sadness_row_id_layout->addSpacerItem(new QSpacerItem(0,6,QSizePolicy::Preferred,QSizePolicy::Preferred));
	sadness_row_id_layout->addWidget(cb_front_row);
	sadness_row_id_layout->addWidget(cb_idChanger);
	sadness_row_id_layout->addSpacerItem(new QSpacerItem(0,6,QSizePolicy::Preferred,QSizePolicy::Preferred));
	sadness_row_id_layout->addWidget(combo_id);

	QHBoxLayout *avatar_name_layout = new QHBoxLayout;
	avatar_name_layout->setContentsMargins(0,0,0,0);
	//avatar_name_layout->setSpacing(0);
	avatar_name_layout->addWidget(lbl_avatar);
	avatar_name_layout->addLayout(name_hp_mp_kills_layout);
	avatar_name_layout->addLayout(sadness_row_id_layout);

	QHBoxLayout *limit_bar_layout = new QHBoxLayout;
	limit_bar_layout->setContentsMargins(0,0,0,0);
	limit_bar_layout->addLayout(limit_level_layout);

	QSpacerItem *limitSpacer = new QSpacerItem(10,0,QSizePolicy::Fixed,QSizePolicy::Fixed);
	limit_bar_layout->addSpacerItem(limitSpacer);
	limit_bar_layout->addWidget(lbl_limit_bar);
	limit_bar_layout->addWidget(slider_limit);
	limit_bar_layout->addWidget(lcd_limit_value);

	QHBoxLayout *str_layout = new QHBoxLayout;
	str_layout->setContentsMargins(0,0,0,0);
	str_layout->addWidget(lbl_str);
	str_layout->addWidget(sb_str);
	str_layout->addWidget(lbl_str_plus_1);
	str_layout->addWidget(sb_str_bonus);
	str_layout->addWidget(lbl_str_plus_2);
	str_layout->addWidget(lbl_str_mat_bonus);
	str_layout->addWidget(lbl_str_equals);
	str_layout->addWidget(lbl_str_total);

	QHBoxLayout *vit_layout = new QHBoxLayout;
	vit_layout->setContentsMargins(0,0,0,0);
	vit_layout->addWidget(lbl_vit);
	vit_layout->addWidget(sb_vit);
	vit_layout->addWidget(lbl_vit_plus_1);
	vit_layout->addWidget(sb_vit_bonus);
	vit_layout->addWidget(lbl_vit_plus_2);
	vit_layout->addWidget(lbl_vit_mat_bonus);
	vit_layout->addWidget(lbl_vit_equals);
	vit_layout->addWidget(lbl_vit_total);

	QHBoxLayout *mag_layout = new QHBoxLayout;
	mag_layout->setContentsMargins(0,0,0,0);
	mag_layout->addWidget(lbl_mag);
	mag_layout->addWidget(sb_mag);
	mag_layout->addWidget(lbl_mag_plus_1);
	mag_layout->addWidget(sb_mag_bonus);
	mag_layout->addWidget(lbl_mag_plus_2);
	mag_layout->addWidget(lbl_mag_mat_bonus);
	mag_layout->addWidget(lbl_mag_equals);
	mag_layout->addWidget(lbl_mag_total);

	QHBoxLayout *spi_layout = new QHBoxLayout;
	spi_layout->setContentsMargins(0,0,0,0);
	spi_layout->addWidget(lbl_spi);
	spi_layout->addWidget(sb_spi);
	spi_layout->addWidget(lbl_spi_plus_1);
	spi_layout->addWidget(sb_spi_bonus);
	spi_layout->addWidget(lbl_spi_plus_2);
	spi_layout->addWidget(lbl_spi_mat_bonus);
	spi_layout->addWidget(lbl_spi_equals);
	spi_layout->addWidget(lbl_spi_total);

	QHBoxLayout *dex_layout = new QHBoxLayout;
	dex_layout->setContentsMargins(0,0,0,0);
	dex_layout->addWidget(lbl_dex);
	dex_layout->addWidget(sb_dex);
	dex_layout->addWidget(lbl_dex_plus_1);
	dex_layout->addWidget(sb_dex_bonus);
	dex_layout->addWidget(lbl_dex_plus_2);
	dex_layout->addWidget(lbl_dex_mat_bonus);
	dex_layout->addWidget(lbl_dex_equals);
	dex_layout->addWidget(lbl_dex_total);

	QHBoxLayout *lck_layout = new QHBoxLayout;
	lck_layout->setContentsMargins(0,0,0,0);
	lck_layout->addWidget(lbl_lck);
	lck_layout->addWidget(sb_lck);
	lck_layout->addWidget(lbl_lck_plus_1);
	lck_layout->addWidget(sb_lck_bonus);
	lck_layout->addWidget(lbl_lck_plus_2);
	lck_layout->addWidget(lbl_lck_mat_bonus);
	lck_layout->addWidget(lbl_lck_equals);
	lck_layout->addWidget(lbl_lck_total);

	QHBoxLayout *base_hp_layout = new QHBoxLayout;
	base_hp_layout->setContentsMargins(0,0,0,0);
	base_hp_layout->addWidget(lbl_base_hp);
	base_hp_layout->addWidget(sb_base_hp);
	base_hp_layout->addWidget(lbl_base_hp_bonus);
	base_hp_layout->addSpacerItem(new QSpacerItem(3,0,QSizePolicy::Expanding,QSizePolicy::Fixed));

	QHBoxLayout *base_mp_layout = new QHBoxLayout;
	base_mp_layout->setContentsMargins(0,0,0,0);
	base_mp_layout->addWidget(lbl_base_mp);
	base_mp_layout->addWidget(sb_base_mp);
	base_mp_layout->addWidget(lbl_base_mp_bonus);
	base_mp_layout->addSpacerItem(new QSpacerItem(3,0,QSizePolicy::Expanding,QSizePolicy::Fixed));

	QHBoxLayout *base_hp_mp_layout = new QHBoxLayout;
	base_hp_mp_layout->setContentsMargins(0,0,0,0);
	base_hp_mp_layout->setSpacing(3);
	base_hp_mp_layout->addLayout(base_hp_layout);
	base_hp_mp_layout->addLayout(base_mp_layout);

	QVBoxLayout *stat_layout = new QVBoxLayout;
	stat_layout->setContentsMargins(0,2,0,0);
	stat_layout->setSpacing(0);
	stat_layout->addLayout(str_layout);
	stat_layout->addLayout(vit_layout);
	stat_layout->addLayout(mag_layout);
	stat_layout->addLayout(spi_layout);
	stat_layout->addLayout(dex_layout);
	stat_layout->addLayout(lck_layout);
	stat_layout->addLayout(base_hp_mp_layout);

	QFrame*stat_box= new QFrame;
	stat_box->setLayout(stat_layout);
	stat_box->adjustSize();

	QLabel *lbl_0x34= new QLabel(QString(tr("0x34")));
	lcd_0x34 = new QLCDNumber;
	lcd_0x34->setDigitCount(2);
    lcd_0x34->setFixedSize(int(32*scale), int(20*scale));
	lcd_0x34->setHexMode();
	lcd_0x34->setSegmentStyle(QLCDNumber::Flat);
	QVBoxLayout *_0x34_layout = new QVBoxLayout;
	_0x34_layout->setContentsMargins(0,0,0,0);
	_0x34_layout->setSpacing(0);
	_0x34_layout->addWidget(lbl_0x34);
	_0x34_layout->addWidget(lcd_0x34);

	QLabel *lbl_0x35= new QLabel(QString(tr("0x35")));
	lcd_0x35 = new QLCDNumber;
	lcd_0x35->setDigitCount(2);
    lcd_0x35->setFixedSize(int (32*scale),int(20*scale));
	lcd_0x35->setHexMode();
	lcd_0x35->setSegmentStyle(QLCDNumber::Flat);
	QVBoxLayout *_0x35_layout = new QVBoxLayout;
	_0x35_layout->setContentsMargins(0,0,0,0);
	_0x35_layout->setSpacing(0);
	_0x35_layout->addWidget(lbl_0x35);
	_0x35_layout->addWidget(lcd_0x35);

	QLabel *lbl_0x36= new QLabel(QString(tr("0x36")));
	lcd_0x36 = new QLCDNumber;
	lcd_0x36->setDigitCount(2);
    lcd_0x36->setFixedSize(int(32*scale),int(20*scale));
	lcd_0x36->setHexMode();
	lcd_0x36->setSegmentStyle(QLCDNumber::Flat);
	QVBoxLayout *_0x36_layout = new QVBoxLayout;
	_0x36_layout->setContentsMargins(0,0,0,0);
	_0x36_layout->setSpacing(0);
	_0x36_layout->addWidget(lbl_0x36);
	_0x36_layout->addWidget(lcd_0x36);

	QLabel *lbl_0x37= new QLabel(QString(tr("0x37")));
	lcd_0x37 = new QLCDNumber;
	lcd_0x37->setDigitCount(2);
    lcd_0x37->setFixedSize(int(32*scale),int(20*scale));
	lcd_0x37->setHexMode();

	lcd_0x37->setSegmentStyle(QLCDNumber::Flat);
	QVBoxLayout *_0x37_layout = new QVBoxLayout;
	_0x37_layout->setContentsMargins(0,0,0,0);
	_0x37_layout->setSpacing(0);
	_0x37_layout->addWidget(lbl_0x37);
	_0x37_layout->addWidget(lcd_0x37);

	QVBoxLayout *unknown_layout = new QVBoxLayout;
	unknown_layout->setContentsMargins(0,0,0,0);
	unknown_layout->setSpacing(0);
	unknown_layout->addLayout(_0x34_layout);
	unknown_layout->addLayout(_0x35_layout);
	unknown_layout->addLayout(_0x36_layout);
	unknown_layout->addLayout(_0x37_layout);

	unknown_box = new QFrame;
	unknown_box->setLayout(unknown_layout);
	unknown_box->setFixedHeight(stat_box->height());
	unknown_box->setVisible(false);

	QHBoxLayout *stat_layout_2 = new QHBoxLayout;
	stat_layout_2->setContentsMargins(0,0,0,0);
	stat_layout_2->addWidget(stat_box);
	stat_layout_2->addWidget(unknown_box);

	QHBoxLayout *limit_use_list = new QHBoxLayout;
	limit_use_list->addLayout(used_limits_layout);
	limit_use_list->addWidget(list_limits);

	QVBoxLayout *limit_box = new QVBoxLayout;
	limit_box->setContentsMargins(0,0,0,0);
	//limit_box->setSpacing(0);
	limit_box->addLayout(limit_bar_layout);
	limit_box->addLayout(limit_use_list);

	QVBoxLayout *lower_section = new QVBoxLayout;
	lower_section->setContentsMargins(0,0,0,0);
	lower_section->setSpacing(0);
    lower_section->addLayout(stat_layout_2);
	lower_section->addLayout(limit_box);

	QVBoxLayout *left_Final = new QVBoxLayout;
	left_Final->setContentsMargins(0,0,0,0);
	//left_Final->setSpacing(0);
	left_Final->addLayout(avatar_name_layout);
	left_Final->addLayout(lower_section);
	left_Final->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Preferred,QSizePolicy::MinimumExpanding));
    QSize iconSize = QSize(fontMetrics().height(),fontMetrics().height());
	weapon_selection = new QComboBox;
    weapon_selection->setIconSize(iconSize);

	armor_selection = new QComboBox;
    armor_selection->setIconSize(iconSize);
    for(int i=256;i<288;i++){armor_selection->addItem(QPixmap::fromImage(Items.image(i)),Items.name(i));}

	accessory_selection = new QComboBox;
    accessory_selection->setIconSize(iconSize);

	for(int i=288;i<320;i++){accessory_selection->addItem(QPixmap::fromImage(Items.image(i)),Items.name(i));}
	accessory_selection->addItem(QPixmap::fromImage(Items.image(288)),tr("-NONE-"));
	materia_edit  = new MateriaEditor(scale,this);
    materia_edit->setStarsSize(int(32*scale));

	elemental_effects = new QListWidget();
	QHBoxLayout *elemental = new QHBoxLayout();
	elemental->setContentsMargins(0,0,0,0);
	elemental->addWidget(elemental_effects);

	elemental_box =new QGroupBox();
	elemental_box->setContentsMargins(3,3,3,3);
	elemental_box->setLayout(elemental);
	elemental_box->setTitle(tr("Elemental Effects"));
	elemental_box->setHidden(1);

	QVBoxLayout *elemental_layout = new QVBoxLayout();
	elemental_layout->setContentsMargins(0,0,0,0);
	elemental_layout->addWidget(elemental_box);
	elemental_layout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding));

	status_effects = new QListWidget();
	QHBoxLayout *status =new QHBoxLayout();
	status->setContentsMargins(0,0,0,0);
	status->addWidget(status_effects);

	status_box = new QGroupBox();
	status_box->setContentsMargins(3,3,3,3);
	status_box->setLayout(status);
	status_box->setTitle(tr("Status Effects"));
	status_box->setHidden(1);

	QVBoxLayout *status_layout = new QVBoxLayout();
	status_layout->setContentsMargins(0,0,0,0);
	status_layout->addWidget(status_box);
	status_layout->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding));

	QHBoxLayout *effects_layout =new QHBoxLayout();
	effects_layout->setContentsMargins(0,0,0,0);
	effects_layout->addLayout(elemental_layout);
	effects_layout->addLayout(status_layout);
	QSpacerItem *spacer2 = new QSpacerItem(-1,-1,QSizePolicy::Expanding,QSizePolicy::Minimum);
	effects_layout->addSpacerItem(spacer2);
	effects_layout->setSpacing(0);

	//set up materia slots and box for weapon.
	weapon_materia_box = new QFrame;
	weapon_materia_box->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
	QSpacerItem *weapon_spacer=new QSpacerItem(-1,-1,QSizePolicy::Expanding,QSizePolicy::Minimum);
	//I Like the No Growth Slots They Look Better so i always use them
	// you can use FF7Item::Style_m_grow_slot() to get the style of that slot

    QSize slotSize = QSize(int(32*scale), int(32*scale));
    QSize linkSize = QSize(int(12*scale), int(16*scale));
	weapon_slot_1 = new QPushButton();
    weapon_slot_1->setFixedSize(slotSize);
    weapon_slot_1->setIconSize(QSize(slotSize));
	weapon_slot_1->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	weapon_slot_1->setHidden(1);

	QHBoxLayout *weapon_slot_1_layout = new QHBoxLayout;
	weapon_slot_1_layout->setContentsMargins(0,0,0,0);
	weapon_slot_1_layout->addWidget(weapon_slot_1);

	weapon_frm_1 = new QFrame;
    weapon_frm_1->setFixedSize(slotSize);
	weapon_frm_1->setFrameShape(QFrame::NoFrame);
	weapon_frm_1->setFrameShadow(QFrame::Plain);
	weapon_frm_1->setLayout(weapon_slot_1_layout);

	weapon_m_link_1= new QLabel();
    weapon_m_link_1->setFixedSize(linkSize);

	weapon_slot_2 = new QPushButton();
    weapon_slot_2->setFixedSize(slotSize);
    weapon_slot_2->setIconSize(QSize(slotSize));
	weapon_slot_2->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	weapon_slot_2->setHidden(1);

	QHBoxLayout *weapon_slot_2_layout = new QHBoxLayout;
	weapon_slot_2_layout->setContentsMargins(0,0,0,0);
	weapon_slot_2_layout->addWidget(weapon_slot_2);

	weapon_frm_2 = new QFrame;
    weapon_frm_2->setFixedSize(slotSize);
	weapon_frm_2->setFrameShape(QFrame::NoFrame);
	weapon_frm_2->setFrameShadow(QFrame::Plain);
	weapon_frm_2->setLayout(weapon_slot_2_layout);

	QHBoxLayout *weapon_slots_1_and_2 = new QHBoxLayout();
	weapon_slots_1_and_2->setContentsMargins(0,0,0,0);
	weapon_slots_1_and_2->addWidget(weapon_frm_1);
	weapon_slots_1_and_2->addWidget(weapon_m_link_1);
	weapon_slots_1_and_2->addWidget(weapon_frm_2);
	weapon_slots_1_and_2->setSpacing(0);

	weapon_slot_3 = new QPushButton();
    weapon_slot_3->setFixedSize(slotSize);
	weapon_slot_3->setFlat(true);
	weapon_slot_3->setAutoFillBackground(true);
    weapon_slot_3->setIconSize(QSize(slotSize));
	weapon_slot_3->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	weapon_slot_3->setHidden(1);

	QHBoxLayout *weapon_slot_3_layout = new QHBoxLayout;
	weapon_slot_3_layout->setContentsMargins(0,0,0,0);
	weapon_slot_3_layout->addWidget(weapon_slot_3);

	weapon_frm_3 = new QFrame;
    weapon_frm_3->setFixedSize(slotSize);
	weapon_frm_3->setFrameShape(QFrame::NoFrame);
	weapon_frm_3->setFrameShadow(QFrame::Plain);
	weapon_frm_3->setLayout(weapon_slot_3_layout);

	weapon_m_link_2 = new QLabel();
    weapon_m_link_2->setFixedSize(linkSize);

	weapon_slot_4 = new QPushButton();
    weapon_slot_4->setFixedSize(slotSize);
	weapon_slot_1->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	weapon_slot_4->setFlat(true);
	weapon_slot_4->setAutoFillBackground(true);
    weapon_slot_4->setIconSize(QSize(slotSize));
	weapon_slot_4->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	weapon_slot_4->setHidden(1);

	QHBoxLayout *weapon_slot_4_layout = new QHBoxLayout;
	weapon_slot_4_layout->setContentsMargins(0,0,0,0);
	weapon_slot_4_layout->addWidget(weapon_slot_4);

	weapon_frm_4 = new QFrame;
    weapon_frm_4->setFixedSize(slotSize);
	weapon_frm_4->setFrameShape(QFrame::NoFrame);
	weapon_frm_4->setFrameShadow(QFrame::Plain);
	weapon_frm_4->setLayout(weapon_slot_4_layout);

	QHBoxLayout *weapon_slots_3_and_4 = new QHBoxLayout();
	weapon_slots_3_and_4->setContentsMargins(0,0,0,0);
	weapon_slots_3_and_4->addWidget(weapon_frm_3);
	weapon_slots_3_and_4->addWidget(weapon_m_link_2);
	weapon_slots_3_and_4->addWidget(weapon_frm_4);
	weapon_slots_3_and_4->setSpacing(0);

	weapon_slot_5 = new QPushButton();
    weapon_slot_5->setFixedSize(slotSize);
	weapon_slot_5->setFlat(true);
	weapon_slot_5->setAutoFillBackground(true);
    weapon_slot_5->setIconSize(QSize(slotSize));
	weapon_slot_5->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	weapon_slot_5->setHidden(1);

	QHBoxLayout *weapon_slot_5_layout = new QHBoxLayout;
	weapon_slot_5_layout->setContentsMargins(0,0,0,0);
	weapon_slot_5_layout->addWidget(weapon_slot_5);

	weapon_frm_5 = new QFrame;
    weapon_frm_5->setFixedSize(slotSize);
	weapon_frm_5->setFrameShape(QFrame::NoFrame);
	weapon_frm_5->setFrameShadow(QFrame::Plain);
	weapon_frm_5->setLayout(weapon_slot_5_layout);

	weapon_m_link_3 = new QLabel();
    weapon_m_link_3->setFixedSize(linkSize);

	weapon_slot_6 = new QPushButton();
    weapon_slot_6->setFixedSize(slotSize);
	weapon_slot_6->setFlat(true);
	weapon_slot_6->setAutoFillBackground(true);
    weapon_slot_6->setIconSize(QSize(slotSize));
	weapon_slot_6->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	weapon_slot_6->setHidden(1);

	QHBoxLayout *weapon_slot_6_layout = new QHBoxLayout;
	weapon_slot_6_layout->setContentsMargins(0,0,0,0);
	weapon_slot_6_layout->addWidget(weapon_slot_6);

	weapon_frm_6 = new QFrame;
    weapon_frm_6->setFixedSize(slotSize);
	weapon_frm_6->setFrameShape(QFrame::NoFrame);
	weapon_frm_6->setFrameShadow(QFrame::Plain);
	weapon_frm_6->setLayout(weapon_slot_6_layout);

	QHBoxLayout *weapon_slots_5_and_6 = new QHBoxLayout();
	weapon_slots_5_and_6->setContentsMargins(0,0,0,0);
	weapon_slots_5_and_6->addWidget(weapon_frm_5);
	weapon_slots_5_and_6->addWidget(weapon_m_link_3);
	weapon_slots_5_and_6->addWidget(weapon_frm_6);
	weapon_slots_5_and_6->setSpacing(0);

	weapon_slot_7 = new QPushButton();
    weapon_slot_7->setFixedSize(slotSize);
	weapon_slot_7->setFlat(true);
	weapon_slot_7->setAutoFillBackground(true);
    weapon_slot_7->setIconSize(QSize(slotSize));
	weapon_slot_7->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	weapon_slot_7->setHidden(1);

	QHBoxLayout *weapon_slot_7_layout = new QHBoxLayout;
	weapon_slot_7_layout->setContentsMargins(0,0,0,0);
	weapon_slot_7_layout->addWidget(weapon_slot_7);

	weapon_frm_7 = new QFrame;
    weapon_frm_7->setFixedSize(slotSize);
	weapon_frm_7->setFrameShape(QFrame::NoFrame);
	weapon_frm_7->setFrameShadow(QFrame::Plain);
	weapon_frm_7->setLayout(weapon_slot_7_layout);

	weapon_m_link_4 = new QLabel();
    weapon_m_link_4->setFixedSize(linkSize);

	weapon_slot_8 = new QPushButton();
    weapon_slot_8->setFixedSize(slotSize);
	weapon_slot_8->setFlat(true);
	weapon_slot_8->setAutoFillBackground(true);
    weapon_slot_8->setIconSize(QSize(slotSize));
	weapon_slot_8->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	weapon_slot_8->setHidden(1);

	QHBoxLayout *weapon_slot_8_layout = new QHBoxLayout;
	weapon_slot_8_layout->setContentsMargins(0,0,0,0);
	weapon_slot_8_layout->addWidget(weapon_slot_8);

	weapon_frm_8 = new QFrame;
    weapon_frm_8->setFixedSize(slotSize);
	weapon_frm_8->setFrameShape(QFrame::NoFrame);
	weapon_frm_8->setFrameShadow(QFrame::Plain);
	weapon_frm_8->setLayout(weapon_slot_8_layout);

	QHBoxLayout *weapon_slots_7_and_8 = new QHBoxLayout();
	weapon_slots_7_and_8->setContentsMargins(0,0,0,0);
	weapon_slots_7_and_8->addWidget(weapon_frm_7);
	weapon_slots_7_and_8->addWidget(weapon_m_link_4);
	weapon_slots_7_and_8->addWidget(weapon_frm_8);
	weapon_slots_7_and_8->setSpacing(0);

	QHBoxLayout *weapon_materia_slots = new QHBoxLayout();
	weapon_materia_slots->setContentsMargins(0,0,0,0);
	weapon_materia_slots->addLayout(weapon_slots_1_and_2);
	weapon_materia_slots->addLayout(weapon_slots_3_and_4);
	weapon_materia_slots->addLayout(weapon_slots_5_and_6);
	weapon_materia_slots->addLayout(weapon_slots_7_and_8);
	weapon_materia_slots->addSpacerItem(weapon_spacer);
	weapon_materia_slots->setSpacing(12);

	weapon_materia_box->setLayout(weapon_materia_slots);

	lblWeaponStats = new QLabel();

	QVBoxLayout *weapon_layout = new QVBoxLayout;
	weapon_layout->setContentsMargins(0,0,0,0);
	weapon_layout->addWidget(weapon_selection);
	weapon_layout->addWidget(lblWeaponStats);
	weapon_layout->addWidget(weapon_materia_box);

	weapon_box = new QGroupBox(tr("Weapon"));
	weapon_box->setLayout(weapon_layout);
	weapon_box->adjustSize();
	//set up materia slots and box for armor.
	armor_materia_box = new QFrame;

	lblArmorStats = new QLabel();

	QSpacerItem *armor_spacer=new QSpacerItem(-1,-1,QSizePolicy::Expanding,QSizePolicy::Minimum);
	//I Like the No Growth Slots They Look Better so i always use them
	// you can use FF7Item::Style_m_grow_slot() to get the style of that slot

	armor_slot_1 = new QPushButton();
    armor_slot_1->setFixedSize(slotSize);
    armor_slot_1->setIconSize(QSize(slotSize));
	armor_slot_1->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	armor_slot_1->setHidden(1);

	QHBoxLayout *armor_slot_1_layout = new QHBoxLayout;
	armor_slot_1_layout->setContentsMargins(0,0,0,0);
	armor_slot_1_layout->addWidget(armor_slot_1);

	armor_frm_1 = new QFrame;
    armor_frm_1->setFixedSize(slotSize);
	armor_frm_1->setFrameShape(QFrame::NoFrame);
	armor_frm_1->setFrameShadow(QFrame::Plain);
	armor_frm_1->setLayout(armor_slot_1_layout);

	armor_m_link_1= new QLabel();
    armor_m_link_1->setFixedSize(linkSize);

	armor_slot_2 = new QPushButton();
    armor_slot_2->setFixedSize(slotSize);
    armor_slot_2->setIconSize(QSize(slotSize));
	armor_slot_2->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	armor_slot_2->setHidden(1);

	QHBoxLayout *armor_slot_2_layout = new QHBoxLayout;
	armor_slot_2_layout->setContentsMargins(0,0,0,0);
	armor_slot_2_layout->addWidget(armor_slot_2);

	armor_frm_2 = new QFrame;
    armor_frm_2->setFixedSize(slotSize);
	armor_frm_2->setFrameShape(QFrame::NoFrame);
	armor_frm_2->setFrameShadow(QFrame::Plain);
	armor_frm_2->setLayout(armor_slot_2_layout);

	QHBoxLayout *armor_slots_1_and_2 = new QHBoxLayout();
	armor_slots_1_and_2->setContentsMargins(0,0,0,0);
	armor_slots_1_and_2->addWidget(armor_frm_1);
	armor_slots_1_and_2->addWidget(armor_m_link_1);
	armor_slots_1_and_2->addWidget(armor_frm_2);
	armor_slots_1_and_2->setSpacing(0);

	armor_slot_3 = new QPushButton();
    armor_slot_3->setFixedSize(slotSize);
	armor_slot_3->setFlat(true);
	armor_slot_3->setAutoFillBackground(true);
    armor_slot_3->setIconSize(QSize(slotSize));
	armor_slot_3->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	armor_slot_3->setHidden(1);

	QHBoxLayout *armor_slot_3_layout = new QHBoxLayout;
	armor_slot_3_layout->setContentsMargins(0,0,0,0);
	armor_slot_3_layout->addWidget(armor_slot_3);

	armor_frm_3 = new QFrame;
    armor_frm_3->setFixedSize(slotSize);
	armor_frm_3->setFrameShape(QFrame::NoFrame);
	armor_frm_3->setFrameShadow(QFrame::Plain);
	armor_frm_3->setLayout(armor_slot_3_layout);

	armor_m_link_2 = new QLabel();
    armor_m_link_2->setFixedSize(linkSize);
	armor_m_link_2->setScaledContents(1);

	armor_slot_4 = new QPushButton();
    armor_slot_4->setFixedSize(slotSize);
	armor_slot_1->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	armor_slot_4->setFlat(true);
	armor_slot_4->setAutoFillBackground(true);
    armor_slot_4->setIconSize(QSize(slotSize));
	armor_slot_4->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	armor_slot_4->setHidden(1);

	QHBoxLayout *armor_slot_4_layout = new QHBoxLayout;
	armor_slot_4_layout->setContentsMargins(0,0,0,0);
	armor_slot_4_layout->addWidget(armor_slot_4);

	armor_frm_4 = new QFrame;
    armor_frm_4->setFixedSize(slotSize);
	armor_frm_4->setFrameShape(QFrame::NoFrame);
	armor_frm_4->setFrameShadow(QFrame::Plain);
	armor_frm_4->setLayout(armor_slot_4_layout);

	QHBoxLayout *armor_slots_3_and_4 = new QHBoxLayout();
	armor_slots_3_and_4->setContentsMargins(0,0,0,0);
	armor_slots_3_and_4->addWidget(armor_frm_3);
	armor_slots_3_and_4->addWidget(armor_m_link_2);
	armor_slots_3_and_4->addWidget(armor_frm_4);
	armor_slots_3_and_4->setSpacing(0);

	armor_slot_5 = new QPushButton();
    armor_slot_5->setFixedSize(slotSize);
	armor_slot_5->setFlat(true);
	armor_slot_5->setAutoFillBackground(true);
    armor_slot_5->setIconSize(QSize(slotSize));
	armor_slot_5->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	armor_slot_5->setHidden(1);

	QHBoxLayout *armor_slot_5_layout = new QHBoxLayout;
	armor_slot_5_layout->setContentsMargins(0,0,0,0);
	armor_slot_5_layout->addWidget(armor_slot_5);

	armor_frm_5 = new QFrame;
    armor_frm_5->setFixedSize(slotSize);
	armor_frm_5->setFrameShape(QFrame::NoFrame);
	armor_frm_5->setFrameShadow(QFrame::Plain);
	armor_frm_5->setLayout(armor_slot_5_layout);

	armor_m_link_3 = new QLabel();
    armor_m_link_3->setFixedSize(linkSize);
	armor_m_link_3->setScaledContents(1);

	armor_slot_6 = new QPushButton();
    armor_slot_6->setFixedSize(slotSize);
	armor_slot_6->setFlat(true);
	armor_slot_6->setAutoFillBackground(true);
    armor_slot_6->setIconSize(QSize(slotSize));
	armor_slot_6->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	armor_slot_6->setHidden(1);

	QHBoxLayout *armor_slot_6_layout = new QHBoxLayout;
	armor_slot_6_layout->setContentsMargins(0,0,0,0);
	armor_slot_6_layout->addWidget(armor_slot_6);

	armor_frm_6 = new QFrame;
    armor_frm_6->setFixedSize(slotSize);
	armor_frm_6->setFrameShape(QFrame::NoFrame);
	armor_frm_6->setFrameShadow(QFrame::Plain);
	armor_frm_6->setLayout(armor_slot_6_layout);

	QHBoxLayout *armor_slots_5_and_6 = new QHBoxLayout();
	armor_slots_5_and_6->setContentsMargins(0,0,0,0);
	armor_slots_5_and_6->addWidget(armor_frm_5);
	armor_slots_5_and_6->addWidget(armor_m_link_3);
	armor_slots_5_and_6->addWidget(armor_frm_6);
	armor_slots_5_and_6->setSpacing(0);

	armor_slot_7 = new QPushButton();
    armor_slot_7->setFixedSize(slotSize);
	armor_slot_7->setFlat(true);
	armor_slot_7->setAutoFillBackground(true);
    armor_slot_7->setIconSize(QSize(slotSize));
	armor_slot_7->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	armor_slot_7->setHidden(1);

	QHBoxLayout *armor_slot_7_layout = new QHBoxLayout;
	armor_slot_7_layout->setContentsMargins(0,0,0,0);
	armor_slot_7_layout->addWidget(armor_slot_7);

	armor_frm_7 = new QFrame;
    armor_frm_7->setFixedSize(slotSize);
	armor_frm_7->setFrameShape(QFrame::NoFrame);
	armor_frm_7->setFrameShadow(QFrame::Plain);
	armor_frm_7->setLayout(armor_slot_7_layout);

	armor_m_link_4 = new QLabel();
    armor_m_link_4->setFixedSize(linkSize);
	armor_m_link_4->setScaledContents(1);

	armor_slot_8 = new QPushButton();
    armor_slot_8->setFixedSize(slotSize);
	armor_slot_8->setFlat(true);
	armor_slot_8->setAutoFillBackground(true);
    armor_slot_8->setIconSize(QSize(slotSize));
	armor_slot_8->setStyleSheet(Items.styleMateriaSlotNoGrowth());
	armor_slot_8->setHidden(1);

	QHBoxLayout *armor_slot_8_layout = new QHBoxLayout;
	armor_slot_8_layout->setContentsMargins(0,0,0,0);
	armor_slot_8_layout->addWidget(armor_slot_8);

	armor_frm_8 = new QFrame;
    armor_frm_8->setFixedSize(slotSize);
	armor_frm_8->setFrameShape(QFrame::NoFrame);
	armor_frm_8->setFrameShadow(QFrame::Plain);
	armor_frm_8->setLayout(armor_slot_8_layout);

	QHBoxLayout *armor_slots_7_and_8 = new QHBoxLayout();
	armor_slots_7_and_8->setContentsMargins(0,0,0,0);
	armor_slots_7_and_8->addWidget(armor_frm_7);
	armor_slots_7_and_8->addWidget(armor_m_link_4);
	armor_slots_7_and_8->addWidget(armor_frm_8);
	armor_slots_7_and_8->setSpacing(0);

	QHBoxLayout *armor_materia_slots = new QHBoxLayout();
	armor_materia_slots->setContentsMargins(0,0,0,0);
	armor_materia_slots->addLayout(armor_slots_1_and_2);
	armor_materia_slots->addLayout(armor_slots_3_and_4);
	armor_materia_slots->addLayout(armor_slots_5_and_6);
	armor_materia_slots->addLayout(armor_slots_7_and_8);
	armor_materia_slots->addSpacerItem(armor_spacer);
	armor_materia_slots->setSpacing(12);

	armor_materia_box->setLayout(armor_materia_slots);

	QVBoxLayout *armor_layout = new QVBoxLayout;
	armor_layout->setContentsMargins(0,0,0,0);
	armor_layout->addWidget(armor_selection);
	armor_layout->addWidget(lblArmorStats);
	armor_layout->addWidget(armor_materia_box);

	armor_box = new QGroupBox(tr("Armor"));
	armor_box->setLayout(armor_layout);

	QVBoxLayout *accessory_layout = new QVBoxLayout;
	accessory_layout->setContentsMargins(0,0,0,0);
	accessory_layout->addWidget(accessory_selection);

	accessory_box = new QGroupBox(tr("Accessory"));
	accessory_box->setLayout(accessory_layout);

	QVBoxLayout *right_Top = new QVBoxLayout;
	right_Top->setContentsMargins(0,0,0,0);
	right_Top->addWidget(weapon_box);
	right_Top->addWidget(armor_box);
	right_Top->addWidget(accessory_box);

	QWidget *right_top = new QWidget;
	right_top->setContentsMargins(0,0,0,0);
	right_top->setLayout(right_Top);

	QVBoxLayout *right_bottom = new QVBoxLayout;
	right_bottom->setContentsMargins(0,0,0,0);
	right_bottom->addWidget(materia_edit);
    right_bottom->addLayout(effects_layout);

	QVBoxLayout *right_Final = new QVBoxLayout;
	right_Final->setContentsMargins(3,0,3,0);
	right_Final->addWidget(right_top);
	right_Final->setSpacing(3);
	right_Final->addLayout(right_bottom);

    toolbox = new QToolBox;

    QFrame *tabStatus = new QFrame;
    tabStatus->setLayout(left_Final);
    tabStatus->adjustSize();
    toolbox->addItem(tabStatus,Chars.icon(0),QString(tr("Status Info")));

    QFrame *tabEquipment = new QFrame;
    tabEquipment->setLayout(right_Final);
    tabEquipment->adjustSize();
    toolbox->addItem(tabEquipment,QIcon(QPixmap::fromImage(Items.image(256))),QString(tr("Equipment")));

    QVBoxLayout *toolbox_layout = new QVBoxLayout;
    toolbox_layout->setContentsMargins(0,0,0,0);
    toolbox_layout->addWidget(toolbox);

    this->setLayout(toolbox_layout);
}
void CharEditor::setToolBoxStyle(QString stylesheet){toolbox->setStyleSheet(stylesheet);}
void CharEditor::setMateriaHoverStyle(QString hoverColor){materia_edit->setHoverStyle(hoverColor);}
void CharEditor::setSliderStyle(QString style){slider_limit->setStyleSheet(style);}

void CharEditor::init_connections()
{
	connect(cb_idChanger,SIGNAL(toggled(bool)),this,SLOT(cb_idChanger_toggled(bool)));
	connect(sb_level,SIGNAL(valueChanged(int)),this,SLOT(Level_Changed(int)));
	connect(sb_curMp,SIGNAL(valueChanged(int)),this,SLOT(setCurMp(int)));
	connect(sb_curHp,SIGNAL(valueChanged(int)),this,SLOT(setCurHp(int)));
	connect(sb_maxMp,SIGNAL(valueChanged(int)),this,SLOT(setMaxMp(int)));
	connect(sb_maxHp,SIGNAL(valueChanged(int)),this,SLOT(setMaxHp(int)));
	connect(sb_base_hp,SIGNAL(valueChanged(int)),this,SLOT(setBaseHp(int)));
	connect(sb_base_mp,SIGNAL(valueChanged(int)),this,SLOT(setBaseMp(int)));
	connect(sb_kills,SIGNAL(valueChanged(int)),this,SLOT(setKills(int)));
	connect(line_name,SIGNAL(textChanged(QString)),this,SLOT(setName(QString)));
	connect(cb_front_row,SIGNAL(toggled(bool)),this,SLOT(setRow(bool)));
	connect(cb_fury,SIGNAL(toggled(bool)),this,SLOT(cb_fury_toggled(bool)));
	connect(cb_sadness,SIGNAL(toggled(bool)),this,SLOT(cb_sadness_toggled(bool)));
	connect(sb_str,SIGNAL(valueChanged(int)),this,SLOT(setStr(int)));
	connect(sb_str_bonus,SIGNAL(valueChanged(int)),this,SLOT(setStrBonus(int)));
	connect(sb_vit,SIGNAL(valueChanged(int)),this,SLOT(setVit(int)));
	connect(sb_vit_bonus,SIGNAL(valueChanged(int)),this,SLOT(setVitBonus(int)));
	connect(sb_mag,SIGNAL(valueChanged(int)),this,SLOT(setMag(int)));
	connect(sb_mag_bonus,SIGNAL(valueChanged(int)),this,SLOT(setMagBonus(int)));
	connect(sb_spi,SIGNAL(valueChanged(int)),this,SLOT(setSpi(int)));
	connect(sb_spi_bonus,SIGNAL(valueChanged(int)),this,SLOT(setSpiBonus(int)));
	connect(sb_dex,SIGNAL(valueChanged(int)),this,SLOT(setDex(int)));
	connect(sb_dex_bonus,SIGNAL(valueChanged(int)),this,SLOT(setDexBonus(int)));
	connect(sb_lck,SIGNAL(valueChanged(int)),this,SLOT(setLck(int)));
	connect(sb_lck_bonus,SIGNAL(valueChanged(int)),this,SLOT(setLckBonus(int)));
	connect(slider_limit,SIGNAL(valueChanged(int)),this,SLOT(setLimitBar(int)));
	connect(sb_total_exp,SIGNAL(valueChanged(int)),this,SLOT(Exp_Changed(int)));
	connect(slider_limit,SIGNAL(valueChanged(int)),lcd_limit_value,SLOT(display(int)));
	connect(list_limits,SIGNAL(clicked(QModelIndex)),this,SLOT(calc_limit_value(QModelIndex)));
	connect(sb_uses_limit_1_1,SIGNAL(valueChanged(int)),this,SLOT(setTimesused1(int))); //Vegeta_Ss4: Fixed limit timeused GUI
	connect(sb_uses_limit_2_1,SIGNAL(valueChanged(int)),this,SLOT(setTimesused2(int))); //Vegeta_Ss4: Fixed limit timeused GUI
	connect(sb_uses_limit_3_1,SIGNAL(valueChanged(int)),this,SLOT(setTimesused3(int))); //Vegeta_Ss4: Fixed limit timeused GUI
	connect(sb_limit_level,SIGNAL(valueChanged(int)),this,SLOT(setLimitLevel(int))); //Vegeta_Ss4: Fixed limitlevel GUI
	connect(combo_id,SIGNAL(currentIndexChanged(int)),this,SLOT(setId(int)));
	connect(weapon_selection,SIGNAL(currentIndexChanged(int)),this,SLOT(setWeapon(int)));
	connect(armor_selection,SIGNAL(currentIndexChanged(int)),this,SLOT(setArmor(int)));
	connect(accessory_selection,SIGNAL(currentIndexChanged(int)),this,SLOT(setAccessory(int)));
	connect(weapon_slot_1,SIGNAL(clicked()),this,SLOT(weapon_slot_1_clicked()));
	connect(weapon_slot_2,SIGNAL(clicked()),this,SLOT(weapon_slot_2_clicked()));
	connect(weapon_slot_3,SIGNAL(clicked()),this,SLOT(weapon_slot_3_clicked()));
	connect(weapon_slot_4,SIGNAL(clicked()),this,SLOT(weapon_slot_4_clicked()));
	connect(weapon_slot_5,SIGNAL(clicked()),this,SLOT(weapon_slot_5_clicked()));
	connect(weapon_slot_6,SIGNAL(clicked()),this,SLOT(weapon_slot_6_clicked()));
	connect(weapon_slot_7,SIGNAL(clicked()),this,SLOT(weapon_slot_7_clicked()));
	connect(weapon_slot_8,SIGNAL(clicked()),this,SLOT(weapon_slot_8_clicked()));
	connect(armor_slot_1,SIGNAL(clicked()),this,SLOT(armor_slot_1_clicked()));
	connect(armor_slot_2,SIGNAL(clicked()),this,SLOT(armor_slot_2_clicked()));
	connect(armor_slot_3,SIGNAL(clicked()),this,SLOT(armor_slot_3_clicked()));
	connect(armor_slot_4,SIGNAL(clicked()),this,SLOT(armor_slot_4_clicked()));
	connect(armor_slot_5,SIGNAL(clicked()),this,SLOT(armor_slot_5_clicked()));
	connect(armor_slot_6,SIGNAL(clicked()),this,SLOT(armor_slot_6_clicked()));
	connect(armor_slot_7,SIGNAL(clicked()),this,SLOT(armor_slot_7_clicked()));
	connect(armor_slot_8,SIGNAL(clicked()),this,SLOT(armor_slot_8_clicked()));
	connect(materia_edit,SIGNAL(ap_changed(qint32)),this,SLOT(matAp_changed(qint32)));
	connect(materia_edit,SIGNAL(id_changed(qint8)),this,SLOT(matId_changed(qint8)));
}
void CharEditor::disconnectAll(void)
{
	disconnect(cb_idChanger,SIGNAL(toggled(bool)),this,SLOT(cb_idChanger_toggled(bool)));
	disconnect(sb_level,SIGNAL(valueChanged(int)),this,SLOT(Level_Changed(int)));
	disconnect(sb_curMp,SIGNAL(valueChanged(int)),this,SLOT(setCurMp(int)));
	disconnect(sb_curHp,SIGNAL(valueChanged(int)),this,SLOT(setCurHp(int)));
	disconnect(sb_maxMp,SIGNAL(valueChanged(int)),this,SLOT(setMaxMp(int)));
	disconnect(sb_maxHp,SIGNAL(valueChanged(int)),this,SLOT(setMaxHp(int)));
	disconnect(sb_base_hp,SIGNAL(valueChanged(int)),this,SLOT(setBaseHp(int)));
	disconnect(sb_base_mp,SIGNAL(valueChanged(int)),this,SLOT(setBaseMp(int)));
	disconnect(sb_kills,SIGNAL(valueChanged(int)),this,SLOT(setKills(int)));
	disconnect(line_name,SIGNAL(textChanged(QString)),this,SLOT(setName(QString)));
	disconnect(cb_front_row,SIGNAL(toggled(bool)),this,SLOT(setRow(bool)));
	disconnect(cb_fury,SIGNAL(toggled(bool)),this,SLOT(cb_fury_toggled(bool)));
	disconnect(cb_sadness,SIGNAL(toggled(bool)),this,SLOT(cb_sadness_toggled(bool)));
	disconnect(sb_str,SIGNAL(valueChanged(int)),this,SLOT(setStr(int)));
	disconnect(sb_str_bonus,SIGNAL(valueChanged(int)),this,SLOT(setStrBonus(int)));
	disconnect(sb_vit,SIGNAL(valueChanged(int)),this,SLOT(setVit(int)));
	disconnect(sb_vit_bonus,SIGNAL(valueChanged(int)),this,SLOT(setVitBonus(int)));
	disconnect(sb_mag,SIGNAL(valueChanged(int)),this,SLOT(setMag(int)));
	disconnect(sb_mag_bonus,SIGNAL(valueChanged(int)),this,SLOT(setMagBonus(int)));
	disconnect(sb_spi,SIGNAL(valueChanged(int)),this,SLOT(setSpi(int)));
	disconnect(sb_spi_bonus,SIGNAL(valueChanged(int)),this,SLOT(setSpiBonus(int)));
	disconnect(sb_dex,SIGNAL(valueChanged(int)),this,SLOT(setDex(int)));
	disconnect(sb_dex_bonus,SIGNAL(valueChanged(int)),this,SLOT(setDexBonus(int)));
	disconnect(sb_lck,SIGNAL(valueChanged(int)),this,SLOT(setLck(int)));
	disconnect(sb_lck_bonus,SIGNAL(valueChanged(int)),this,SLOT(setLckBonus(int)));
	disconnect(slider_limit,SIGNAL(valueChanged(int)),this,SLOT(setLimitBar(int)));
	disconnect(sb_total_exp,SIGNAL(valueChanged(int)),this,SLOT(Exp_Changed(int)));
	disconnect(slider_limit,SIGNAL(valueChanged(int)),lcd_limit_value,SLOT(display(int)));
	disconnect(list_limits,SIGNAL(clicked(QModelIndex)),this,SLOT(calc_limit_value(QModelIndex)));
	disconnect(sb_uses_limit_1_1,SIGNAL(valueChanged(int)),this,SLOT(setTimesused1(int))); //Vegeta_Ss4: Fixed limit timeused GUI
	disconnect(sb_uses_limit_2_1,SIGNAL(valueChanged(int)),this,SLOT(setTimesused2(int))); //Vegeta_Ss4: Fixed limit timeused GUI
	disconnect(sb_uses_limit_3_1,SIGNAL(valueChanged(int)),this,SLOT(setTimesused3(int))); //Vegeta_Ss4: Fixed limit timeused GUI
	disconnect(sb_limit_level,SIGNAL(valueChanged(int)),this,SLOT(setLimitLevel(int))); //Vegeta_Ss4: Fixed limitlevel GUI
	disconnect(combo_id,SIGNAL(currentIndexChanged(int)),this,SLOT(setId(int)));
	disconnect(weapon_selection,SIGNAL(currentIndexChanged(int)),this,SLOT(setWeapon(int)));
	disconnect(armor_selection,SIGNAL(currentIndexChanged(int)),this,SLOT(setArmor(int)));
	disconnect(accessory_selection,SIGNAL(currentIndexChanged(int)),this,SLOT(setAccessory(int)));
	disconnect(weapon_slot_1,SIGNAL(clicked()),this,SLOT(weapon_slot_1_clicked()));
	disconnect(weapon_slot_2,SIGNAL(clicked()),this,SLOT(weapon_slot_2_clicked()));
	disconnect(weapon_slot_3,SIGNAL(clicked()),this,SLOT(weapon_slot_3_clicked()));
	disconnect(weapon_slot_4,SIGNAL(clicked()),this,SLOT(weapon_slot_4_clicked()));
	disconnect(weapon_slot_5,SIGNAL(clicked()),this,SLOT(weapon_slot_5_clicked()));
	disconnect(weapon_slot_6,SIGNAL(clicked()),this,SLOT(weapon_slot_6_clicked()));
	disconnect(weapon_slot_7,SIGNAL(clicked()),this,SLOT(weapon_slot_7_clicked()));
	disconnect(weapon_slot_8,SIGNAL(clicked()),this,SLOT(weapon_slot_8_clicked()));
	disconnect(armor_slot_1,SIGNAL(clicked()),this,SLOT(armor_slot_1_clicked()));
	disconnect(armor_slot_2,SIGNAL(clicked()),this,SLOT(armor_slot_2_clicked()));
	disconnect(armor_slot_3,SIGNAL(clicked()),this,SLOT(armor_slot_3_clicked()));
	disconnect(armor_slot_4,SIGNAL(clicked()),this,SLOT(armor_slot_4_clicked()));
	disconnect(armor_slot_5,SIGNAL(clicked()),this,SLOT(armor_slot_5_clicked()));
	disconnect(armor_slot_6,SIGNAL(clicked()),this,SLOT(armor_slot_6_clicked()));
	disconnect(armor_slot_7,SIGNAL(clicked()),this,SLOT(armor_slot_7_clicked()));
	disconnect(armor_slot_8,SIGNAL(clicked()),this,SLOT(armor_slot_8_clicked()));
	disconnect(materia_edit,SIGNAL(ap_changed(qint32)),this,SLOT(matAp_changed(qint32)));
	disconnect(materia_edit,SIGNAL(id_changed(qint8)),this,SLOT(matId_changed(qint8)));
}
quint8 CharEditor::id(){return data.id;}
quint8 CharEditor::level(){return data.level;}
quint8 CharEditor::str(){return data.strength;}
quint8 CharEditor::vit(){return data.vitality;}
quint8 CharEditor::mag(){return data.magic;}
quint8 CharEditor::spi(){return data.spirit;}
quint8 CharEditor::dex(){return data.dexterity;}
quint8 CharEditor::lck(){return data.luck;}
quint8 CharEditor::strBonus(){return data.strength_bonus;}
quint8 CharEditor::vitBonus(){return data.vitality_bonus;}
quint8 CharEditor::magBonus(){return data.magic_bonus;}
quint8 CharEditor::spiBonus(){return data.spirit_bonus;}
quint8 CharEditor::dexBonus(){return data.dexterity_bonus;}
quint8 CharEditor::lckBonus(){return data.luck_bonus;}
qint8 CharEditor::limitLevel(){return data.limitlevel;}
quint8 CharEditor::limitBar(){return data.limitbar;}
QString CharEditor::name(){return line_name->text();}
quint8 CharEditor::weapon(){return data.weapon;}
quint8 CharEditor::armor(){return data.armor;}
quint8 CharEditor::accessory(){return data.accessory;}
quint16 CharEditor::curHp(){return data.curHP;}
quint16 CharEditor::maxHp(){return data.maxHP;}
quint16 CharEditor::curMp(){return data.curMP;}
quint16 CharEditor::maxMp(){return data.maxMP;}
quint16 CharEditor::kills(){return data.kills;}
quint8 CharEditor::row(){return data.flags[1];}
quint8 CharEditor::levelProgress(){return data.flags[2];}
quint8 CharEditor::sadnessfury(){return data.flags[0];}
quint16 CharEditor::limits(){return data.limits;}
quint16 CharEditor::timesused1(){return data.timesused1;}
quint16 CharEditor::timesused2(){return data.timesused2;}
quint16 CharEditor::timesused3(){return data.timesused3;}
quint16 CharEditor::baseHp(){return data.baseHP;}
quint16 CharEditor::baseMp(){return data.baseMP;}
quint32 CharEditor::exp(){return data.exp;}
quint32 CharEditor::expNext(){return data.expNext;}
materia CharEditor::char_materia(int mat){return data.materias[mat];}

void CharEditor::Exp_Changed(int exp)
{
	if(quint32(exp) != data.exp)
	{
		setExp(exp);
		if(autolevel)
		{
			if( (data.exp>=Chars.totalExpForLevel(data.id,data.level)) || (data.exp<=Chars.totalExpForLevel(data.id,data.level-1)) )
			{
				int level=0;
				int prev_level = data.level;
				for (int i=level;i<99;i++)
				{
					if(data.exp>=Chars.totalExpForLevel(data.id,i)){level++;}
				}
				sb_level->blockSignals(true);
				sb_level->setValue(level);
				setLevel(level);
				sb_level->blockSignals(false);
				level_up(prev_level);
			}
			update_tnl_bar();
		}
	}
}

void CharEditor::Level_Changed(int level)
{
	//if(level != data.level)
   // {
		if(autolevel)
		{
			int prev_level=data.level;
			setLevel(level);
			if(level<=0){setExp(0);}
			else{setExp(Chars.totalExpForLevel(data.id,level-1));}
			sb_total_exp->blockSignals(true);
			sb_total_exp->setValue(data.exp);
			sb_total_exp->blockSignals(false);
			level_up(prev_level);
			update_tnl_bar();
		}
		else{setLevel(level);}
   // }
}
void CharEditor::setChar(FF7CHAR Chardata,QString Processed_Name)
{
	disconnectAll();// remove all connections. safer signal blocking!
	data = Chardata;
	_name=Processed_Name;
	//more here like setting the gui stuff.
    lbl_avatar->setPixmap(Chars.pixmap(data.id).scaled(lbl_avatar->width(),lbl_avatar->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
	line_name->setText(_name);
	sb_level->setValue(data.level);
	sb_curMp->setValue(data.curMP);
	sb_curHp->setValue(data.curHP);
	sb_maxHp->setValue(data.maxHP);
	lbl_hp_max->setText(QString(" %1").arg(QString::number(data.maxHP)));
	sb_maxMp->setValue(data.maxMP);
	lbl_mp_max->setText(QString(" %1").arg(QString::number(data.maxMP)));
	sb_kills->setValue(data.kills);
	combo_id->setCurrentIndex(data.id);
	if(data.flags[0]==FF7Char::Fury){cb_fury->setChecked(Qt::Checked);}else{cb_fury->setChecked(Qt::Unchecked);}
	if(data.flags[0]==FF7Char::Sadness ){cb_sadness->setChecked(Qt::Checked);}else{cb_sadness->setChecked(Qt::Unchecked);}
	if(data.flags[1] ==FF7Char::FrontRow){cb_front_row->setChecked(Qt::Checked);}else{cb_front_row->setChecked(Qt::Unchecked);}
	sb_total_exp->setValue(data.exp);
	lbl_level_next->setText(tr("Next: %1").arg(QString::number(data.expNext)));
	slider_limit->setValue(data.limitbar);
	lcd_limit_value->display(int(data.limitbar));
	bar_tnl->setValue(data.flags[2]);
	sb_str->setValue(data.strength);
	sb_str_bonus->setValue(data.strength_bonus);
	sb_vit->setValue(data.vitality);
	sb_vit_bonus->setValue(data.vitality_bonus);
	sb_dex->setValue(data.dexterity);
	sb_dex_bonus->setValue(data.dexterity_bonus);
	sb_mag->setValue(data.magic);
	sb_mag_bonus->setValue(data.magic_bonus);
	sb_spi->setValue(data.spirit);
	sb_spi_bonus->setValue(data.spirit_bonus);
	sb_lck->setValue(data.luck);
	sb_lck_bonus->setValue(data.luck_bonus);
	sb_base_hp->setValue(data.baseHP);
	sb_base_mp->setValue(data.baseMP);

	if(data.id ==FF7Char::CaitSith || data.id ==FF7Char::Vincent ||data.id ==FF7Char::YoungCloud || data.id ==FF7Char::Sephiroth )
	{
		if(!advancedMode){cb_idChanger->setHidden(false);}
		else{cb_idChanger->setHidden(true);}
		if(data.id == FF7Char::CaitSith || data.id == FF7Char::YoungCloud){cb_idChanger->setText(tr("Young Cloud"));}
		if(data.id ==FF7Char::Vincent || data.id == FF7Char::Sephiroth){cb_idChanger->setText(tr("Sephiroth"));}
		if(data.id == FF7Char::CaitSith || data.id == FF7Char::Vincent){cb_idChanger->setCheckState(Qt::Unchecked);}
		if(data.id == FF7Char::YoungCloud || data.id == FF7Char::Sephiroth){cb_idChanger->setCheckState(Qt::Checked);}
	}
	else{cb_idChanger->setHidden(true);}
	//Process the limits.
	list_limits->clear();
	list_limits->addItems(Chars.limits(data.id));
	for(int i=0;i<7;i++)
	{//Process the List. Hide "" entries, and Check Limts Learned.

		if(list_limits->item(i)->text()=="") {list_limits->item(i)->setHidden(true);}
		else{list_limits->item(i)->setHidden(false);}

		if(data.limits & (1<< _limitbitarray[i])){list_limits->item(i)->setCheckState(Qt::Checked);} //Vegeta_Ss4: Fixed list_limits GUI
		else{list_limits->item(i)->setCheckState(Qt::Unchecked);}
	}
	list_limits->setFixedHeight(list_limits->sizeHintForRow(0)*list_limits->count() +list_limits->contentsMargins().top()+list_limits->contentsMargins().bottom());
	sb_uses_limit_1_1->setValue(data.timesused1); //Vegeta_Ss4: Fixed limit timeused GUI
	sb_uses_limit_2_1->setValue(data.timesused2); //Vegeta_Ss4: Fixed limit timeused GUI
	sb_uses_limit_3_1->setValue(data.timesused3); //Vegeta_Ss4: Fixed limit timeused GUI
	sb_limit_level->setValue(data.limitlevel); //Vegeta_Ss4: Fixed limitlevel GUI

	quint8 weapon = data.weapon;
	weapon_selection->clear();

	for(int i = Chars.weaponStartingId(data.id); i < Chars.numberOfWeapons(data.id)+Chars.weaponStartingId(data.id);i++)
	{
		weapon_selection->addItem(QPixmap::fromImage(Items.image(i)),Items.name(i));
	}

	data.weapon = weapon;
	if(data.id !=FF7Char::Sephiroth){weapon_selection->setCurrentIndex(data.weapon-Chars.weaponOffset(data.id));}
	else{weapon_selection->blockSignals(true);weapon_selection->setCurrentIndex(0);weapon_selection->blockSignals(false);}

	armor_selection->setCurrentIndex(data.armor);

	if(data.accessory != FF7Char::EmptyAccessory){accessory_selection->setCurrentIndex(data.accessory);}
	else{accessory_selection->setCurrentIndex(32);}
	//set the unknowns
	lcd_0x34->display(data.z_4[0]);
	lcd_0x35->display(data.z_4[1]);
	lcd_0x36->display(data.z_4[2]);
	lcd_0x37->display(data.z_4[3]);
	calc_stats();
	update_materia_slots();
	elemental_info();
	status_info();
	mButtonPress(mslotsel);
	init_connections();//reconnect all
}

void CharEditor::setLevel(int level)
{
	if(data.level == level){return;}
	else
	{
		if(level<0){data.level=0;}
		else if(level>99){data.level=99;}
		else{data.level=level;}
		emit level_changed(data.level);
	}
}
void CharEditor::cb_sadness_toggled(bool sad)
{
	if(sad)
	{
		setSadnessFury(FF7Char::Sadness);
		cb_fury->blockSignals(true);
		cb_fury->setChecked(Qt::Unchecked);
		cb_fury->blockSignals(false);
	}
	else{setSadnessFury(0);}
}
void CharEditor::cb_fury_toggled(bool fury)
{
	if(fury)
	{
		setSadnessFury(FF7Char::Fury);
		cb_sadness->blockSignals(true);
		cb_sadness->setChecked(Qt::Unchecked);
		cb_sadness->blockSignals(false);
	}
	else{setSadnessFury(0);}
}
void CharEditor::setMaxHp(int maxHp)
{
	if(data.maxHP == maxHp){return;}
	else
	{
		if(maxHp<0){data.maxHP=0;}
		else if(maxHp >32767){data.maxHP=32767;}
		else{data.maxHP=maxHp;}
		emit maxHp_changed(data.maxHP);
	}
}
void CharEditor::setCurHp(int curHp)
{
	if(data.curHP == curHp){return;}
	else
	{
		if(curHp<0){data.curHP=0;}
		else if(curHp >32767){data.curHP=32767;}
		else{data.curHP=curHp;}
		emit curHp_changed(data.curHP);
	}
}
void CharEditor::setMaxMp(int maxMp)
{
	if(data.maxMP == maxMp){return;}
	else
	{
		if(maxMp<0){data.maxMP=0;}
		else if(maxMp >32767){data.maxMP=32767;}
		else{data.maxMP=maxMp;}
		emit maxMp_changed(data.maxMP);
	}
}
void CharEditor::setCurMp(int curMp)
{
	if(data.curMP == curMp){return;}
	else
	{
		if(curMp<0){data.curMP=0;}
		else if(curMp >32767){data.curMP=32767;}
		else{data.curMP=curMp;}
		emit curMp_changed(data.curMP);
	}
}
void CharEditor::setKills(int kills)
{
	if(data.kills == kills){return;}
	else
	{
		if(kills<0){data.kills=0;}
		else if(kills >65535){data.kills=65535;}
		else{data.kills=kills;}
		emit kills_changed(data.kills);
	}
}
void CharEditor::setName(QString name)
{
	if(_name==name){return;}
	else
	{
		_name=name;
		emit name_changed(_name);
	}
}
void CharEditor::setId(int id)
{
	if(data.id ==id){return;}
	else
	{
		if(id<0){data.id = 0;}
		else if(id>0x0B){data.id=FF7Char::Empty;}
		else{data.id = id;}
		setChar(data,line_name->text());
		emit id_changed(data.id);
	}
}
void CharEditor::setStr(int strength)
{
	if(data.strength ==strength){return;}
	else
	{
		if(strength<0){data.strength=0;}
		else if(strength>0xFF){data.strength=0xFF;}
		else{data.strength = strength;}
		emit str_changed(data.strength);
		calc_stats();
	}
}
void CharEditor::setVit(int vitality)
{
	if(data.vitality ==vitality){return;}
	else
	{
		if(vitality<0){data.vitality = 0;}
		else if(vitality>0xFF){data.vitality=0xFF;}
		else{data.vitality = vitality;}
		emit vit_changed(data.vitality);
		calc_stats();
	}
}
void CharEditor::setMag(int magic)
{
	if(data.magic ==magic){return;}
	else
	{
		if(magic<0){data.magic = 0;}
		else if(magic>0xFF){data.magic=0xFF;}
		else{data.magic = magic;}
		emit mag_changed(data.magic);
		calc_stats();
	}
}
void CharEditor::setSpi(int spirit)
{
	if(data.spirit ==spirit){return;}
	else
	{
		if(spirit<0){data.spirit = 0;}
		else if(spirit>0xFF){data.spirit=0xFF;}
		else{data.spirit = spirit;}
		emit spi_changed(data.spirit);
		calc_stats();
	}
}
void CharEditor::setDex(int dexterity)
{
	if(data.dexterity ==dexterity){return;}
	else
	{
		if(dexterity<0){data.dexterity = 0;}
		else if(dexterity>0xFF){data.dexterity=0xFF;}
		else{data.dexterity = dexterity;}
		emit dex_changed(data.dexterity);
		calc_stats();
	}
}
void CharEditor::setLck(int luck)
{
	if(data.luck ==luck){return;}
	else
	{
		if(luck<0){data.luck = 0;}
		else if(luck>0xFF){data.luck=0xFF;}
		else{data.luck = luck;}
		emit lck_changed(data.luck);
		calc_stats();
	}
}
void CharEditor::setStrBonus(int strength_bonus)
{
	if(data.strength_bonus ==strength_bonus){return;}
	else
	{
		if(strength_bonus<0){data.strength_bonus = 0;}
		else if(strength_bonus>0xFF){data.strength_bonus=0xFF;}
		else{data.strength_bonus = strength_bonus;}
		emit strBonus_changed(data.strength_bonus);
		calc_stats();
	}
}
void CharEditor::setVitBonus(int vitality_bonus)
{
	if(data.vitality_bonus ==vitality_bonus){return;}
	else
	{
		if(vitality_bonus<0){data.vitality_bonus = 0;}
		else if(vitality_bonus>0xFF){data.vitality_bonus=0xFF;}
		else{data.vitality_bonus = vitality_bonus;}
		emit vitBonus_changed(data.vitality_bonus);
		calc_stats();
	}
}
void CharEditor::setMagBonus(int magic_bonus)
{
	if(data.magic_bonus ==magic_bonus){return;}
	else
	{
		if(magic_bonus<0){data.magic_bonus = 0;}
		else if(magic_bonus>0xFF){data.magic_bonus=0xFF;}
		else{data.magic_bonus = magic_bonus;}
		emit magBonus_changed(data.magic_bonus);
		calc_stats();
	}
}
void CharEditor::setSpiBonus(int spirit_bonus)
{
	if(data.spirit_bonus ==spirit_bonus){return;}
	else
	{
		if(spirit_bonus<0){data.spirit_bonus = 0;}
		else if(spirit_bonus>0xFF){data.spirit_bonus=0xFF;}
		else{data.spirit_bonus = spirit_bonus;}
		emit spiBonus_changed(data.spirit_bonus);
		calc_stats();
	}
}
void CharEditor::setDexBonus(int dexterity_bonus)
{
	if(data.dexterity_bonus ==dexterity_bonus){return;}
	else
	{
		if(dexterity_bonus<0){data.dexterity_bonus = 0;}
		else if(dexterity_bonus>0xFF){data.dexterity_bonus=0xFF;}
		else{data.dexterity_bonus = dexterity_bonus;}
		emit dexBonus_changed(data.dexterity_bonus);
		calc_stats();
	}
}
void CharEditor::setLckBonus(int luck_bonus)
{
	if(data.luck_bonus ==luck_bonus){return;}
	else
	{
		if(luck_bonus<0){data.luck_bonus = 0;}
		else if(luck_bonus>0xFF){data.luck_bonus=0xFF;}
		else{data.luck_bonus = luck_bonus;}
		emit lckBonus_changed(data.luck_bonus);
		calc_stats();
	}
}
void CharEditor::setLimitLevel(int limitlevel)
{
	if(data.limitlevel ==limitlevel){return;}
	else
	{
		if(limitlevel<0){data.limitlevel = 0;}
		else if(limitlevel>4){data.limitlevel=4;}
		else{data.limitlevel = limitlevel;}
		emit limitLevel_changed(data.limitlevel);
	}
}
void CharEditor::setLimitBar(int limitbar)
{
	if(data.limitbar ==limitbar){return;}
	else
	{
		if(limitbar<0){data.limitbar = 0;}
		else if(limitbar>0xFF){data.limitbar=0xFF;}
		else{data.limitbar = limitbar;}
		emit limitBar_changed(data.limitbar);
	}
}
void CharEditor::setWeapon(int weapon)
{
	if(weapon == (data.weapon-Chars.weaponOffset(data.id))){return;}
	else
	{
		if(weapon<0){data.weapon=Chars.weaponOffset(data.id);}
		else if(weapon>Chars.numberOfWeapons(data.id)){data.weapon=Chars.numberOfWeapons(data.id)+Chars.weaponOffset(data.id);}
		else {data.weapon=weapon+Chars.weaponOffset(data.id);}
		emit weapon_changed(data.weapon);
		//Update the Widget.
		elemental_info();
		status_info();
		update_materia_slots();
		calc_stats();
	}
}
void CharEditor::setArmor(int armor)
{
	if(armor == data.armor){return;}
	else
	{
		if(armor<0){data.armor=0;}
		else if(armor>32){data.armor=FF7Char::EmptyArmor;}
		else {data.armor= armor; }
		emit armor_changed(data.armor);
		elemental_info();
		status_info();
		update_materia_slots();
		calc_stats();
	}
}
void CharEditor::setAccessory(int accessory)
{
	if(accessory == data.accessory){return;}
	else
	{
		if(accessory<0){data.accessory=0;}
		else if(accessory>32){data.accessory=FF7Char::EmptyAccessory;}
		else {data.accessory = accessory;}
		emit accessory_changed(data.accessory);
		elemental_info();
		status_info();
		calc_stats();
	}
}
void CharEditor::setSadnessFury(int sad_fury)
{
	if(sad_fury == data.flags[0]){return;}
	else
	{
		if(sad_fury==FF7Char::Fury){data.flags[0]=FF7Char::Fury;}
		else if( sad_fury==FF7Char::Sadness ){data.flags[0]=FF7Char::Sadness;}
		else{data.flags[0]=0;}
		emit sadnessfury_changed(data.flags[0]);
	}

}
void CharEditor::setRow(bool front_row)
{
	if( (front_row) && (data.flags[1]==FF7Char::FrontRow) ){return;}
	else if((!front_row) && (data.flags[1]==FF7Char::BackRow)){return;}
	else
	{
		if(front_row){data.flags[1]=FF7Char::FrontRow;}
		else{data.flags[1]=FF7Char::BackRow;}
		emit row_changed(data.flags[1]);
	}
}

void CharEditor::setLevelProgress(int level_progress)
{//Level progress bar (0-63) game ingores values <4 4-63 are visible as "progress"
	if(level_progress ==data.flags[2]){return;}
	else
	{
		if(level_progress<0){data.flags[2]=0;}
		else if(level_progress >63){data.flags[2]=63;}
		else{data.flags[2]=level_progress;}
		emit levelProgress_changed(data.flags[2]);
	}
}

void CharEditor::setLimits(int limits)
{
	if(limits ==data.limits){return;}
	else
	{
		if(limits <0){data.limits=0;}
		else if(limits>32767){data.limits=32767;}
		else {data.limits = limits;}
		emit limits_changed(data.limits);
	}
}
void CharEditor::setTimesused1(int timesused)
{
	if(timesused ==data.timesused1){return;}
	else
	{
		if(timesused <0){data.timesused1=0;}
		else if(timesused>65535){data.timesused1=65535;}
		else{data.timesused1 = timesused;}
		emit(timesused1_changed(data.timesused1));
	}
}

void CharEditor::setTimesused2(int timesused)
{
	if(timesused ==data.timesused2){return;}
	else
	{
		if(timesused <0){data.timesused2=0;}
		else if(timesused>65535){data.timesused2=65535;}
		else{data.timesused2 = timesused;}
		emit(timesused2_changed(data.timesused2));
	}
}
void CharEditor::setTimesused3(int timesused)
{
	if(timesused ==data.timesused3){return;}
	else
	{
		if(timesused <0){data.timesused3=0;}
		else if(timesused>65535){data.timesused3=65535;}
		else{data.timesused3 = timesused;}
		emit(timesused3_changed(data.timesused3));
	}
}
void CharEditor::setBaseHp(int baseHp)
{
	if(data.baseHP == baseHp){return;}
	else
	{
		if(baseHp<0){data.baseHP=0;}
		else if(baseHp >32767){data.baseHP=32767;}
		else{data.baseHP=baseHp;}
		emit baseHp_changed(data.baseHP);
		calc_stats();
	}
}
void CharEditor::setBaseMp(int baseMp)
{
	if(data.baseMP == baseMp){return;}
	else
	{
		if(baseMp<0){data.baseMP=0;}
		else if(baseMp >32767){data.baseMP=32767;}
		else{data.baseMP=baseMp;}
		emit baseMp_changed(data.baseMP);
		calc_stats();
	}
}
void CharEditor::setExp(int exp)
{
	if(data.exp == quint32(exp)){return;}
	else
	{
		if(exp<0){data.exp=0;}
		else{data.exp=exp;}
		emit exp_changed(data.exp);
	}
}
void CharEditor::setExpNext(int expNext)
{
	if(data.expNext == quint32(expNext)){return;}
	else
	{
		if(expNext<0){data.expNext=0;}
		else{data.expNext=expNext;}
		emit expNext_changed(data.expNext);
	}
}

void CharEditor::calc_limit_value(QModelIndex item)
{
	int row = item.row();
	int limits = data.limits;
	if(list_limits->item(row)->checkState()==Qt::Checked){limits |= (1<< _limitbitarray[row]);}
	else{limits &= ~(1<<_limitbitarray[row]);}
	setLimits(limits);
}
void CharEditor::setAutoLevel(bool ans){autolevel=ans;if(ans){Level_Changed(data.level);}}//used to turn off auto char leveling
bool CharEditor::AutoLevel(void){return autolevel;}
void CharEditor::setAutoStatCalc(bool ans){autostatcalc=ans;calc_stats();}//Toggle stat calculation
bool CharEditor::AutoStatCalc(void){return autostatcalc;}
bool CharEditor::AdvancedMode(void){return advancedMode;}
void CharEditor::setAdvancedMode(bool new_advancedMode)
{
	advancedMode = new_advancedMode;
	unknown_box->setVisible(advancedMode);
	combo_id->setVisible(advancedMode);
	sb_maxHp->setVisible(advancedMode);
	sb_maxMp->setVisible(advancedMode);
	lbl_hp_max->setVisible(!advancedMode);
	lbl_mp_max->setVisible(!advancedMode);
	//if viewing cait/vincent/y.cloud or sephiroth hid the checkbox for simple id changing.
	if(data.id ==FF7Char::CaitSith || data.id ==FF7Char::Vincent ||data.id ==FF7Char::YoungCloud || data.id ==FF7Char::Sephiroth ){cb_idChanger->setHidden(advancedMode);}
}
void CharEditor::setEditable(bool edit)
{
	editable = edit;
	if(editable){
	// unlock all items, do this better later on.
	this->setEnabled(true);
	}
	else{
	//lock all items
	this->setEnabled(false);
	}
}
bool CharEditor::Editable(void){return editable;}

void CharEditor::calc_stats(void)
{
	int str_total=0;
	int vit_total=0;
	int spi_total=0;
	int dex_total=0;
	int mag_total=0;
	int lck_total=0;

	int str_bonus=0;
	int vit_bonus=0;
	int spi_bonus=0;
	int dex_bonus=0;
	int mag_bonus=0;
	int lck_bonus=0;
	int hp_bonus=0;
	int mp_bonus=0;

	str_total = data.strength + data.strength_bonus;
	vit_total= data.vitality + data.vitality_bonus;
	dex_total = data.dexterity + data.dexterity_bonus;
	spi_total = data.spirit + data.spirit_bonus;
	mag_total = data.magic + data.magic_bonus;
	lck_total = data.luck + data.luck_bonus;

	if(autostatcalc)
	{
		//add equipment bonuses
		//Weapon
		str_bonus +=Items.statSTR(data.weapon + 128 );
		vit_bonus +=Items.statVIT(data.weapon + 128);
		dex_bonus +=Items.statDEX(data.weapon + 128);
		spi_bonus +=Items.statSPI(data.weapon + 128);
		mag_bonus +=Items.statMAG(data.weapon + 128);
		lck_bonus +=Items.statLCK(data.weapon + 128);
		hp_bonus+=Items.statHP(data.weapon + 128);
		mp_bonus+=Items.statMP(data.weapon + 128);
		QString title;
		title.append(tr("AP:x%1").arg(QString::number(Items.materiaGrowthRate(data.weapon + 128))));

		if(Items.statSTR(data.weapon + 128)!=0)
		{
			title.append(tr(" Str:+%1").arg(QString::number(Items.statSTR(data.weapon + 128))));
		}
		if(Items.statVIT(data.weapon + 128)!=0)
		{
			title.append(tr(" Vit:+%1").arg(QString::number(Items.statVIT(data.weapon + 128))));
		}
		if(Items.statDEX(data.weapon + 128)!=0)
		{
			title.append(tr(" Dex:+%1").arg(QString::number(Items.statDEX(data.weapon + 128))));
		}
		if(Items.statSPI(data.weapon + 128)!=0)
		{
			title.append(tr(" Spi:+%1").arg(QString::number(Items.statSPI(data.weapon + 128))));
		}
		if(Items.statMAG(data.weapon + 128)!=0)
		{
			title.append(tr(" Mag:+%1").arg(QString::number(Items.statMAG(data.weapon + 128))));
		}
		if(Items.statLCK(data.weapon + 128)!=0)
		{
			title.append(tr(" Lck:+%1").arg(QString::number(Items.statLCK(data.weapon + 128))));
		}
		if(Items.statHP(data.weapon + 128)!=0)
		{
			title.append(tr(" Hp:+%1%").arg(QString::number(Items.statHP(data.weapon + 128))));
		}
		if(Items.statMP(data.weapon + 128)!=0)
		{
			title.append(tr(" Mp:+%1%").arg(QString::number(Items.statMP(data.weapon + 128))));
		}
		lblWeaponStats->setText(title);
		//Armor
		str_bonus += Items.statSTR(data.armor + 256);
		vit_bonus +=Items.statVIT(data.armor + 256);
		dex_bonus +=Items.statDEX(data.armor + 256);
		spi_bonus +=Items.statSPI(data.armor + 256);
		mag_bonus +=Items.statMAG(data.armor + 256);
		lck_bonus +=Items.statLCK(data.armor+ 256);
		hp_bonus+=Items.statHP(data.armor + 256);
		mp_bonus+=Items.statMP(data.armor + 256);
		title.clear();
		title.append(tr("AP:x%1").arg(QString::number(Items.materiaGrowthRate(data.armor + 256))));
		if(Items.statSTR(data.armor + 256) != 0)
		{
			title.append(tr(" Str:+%1").arg(QString::number(Items.statSTR(data.armor + 256))));
		}
		if(Items.statVIT(data.armor + 256) !=0)
		{
			title.append(tr(" Vit:+%1").arg(QString::number(Items.statVIT(data.armor + 256))));
		}
		if(Items.statDEX(data.armor + 256)!=0)
		{
			title.append(tr(" Dex:+%1").arg(QString::number(Items.statDEX(data.armor + 256))));
		}
		if(Items.statSPI(data.armor + 256)!=0)
		{
			title.append(tr(" Spi:+%1").arg(QString::number(Items.statSPI(data.armor + 256))));
		}
		if(Items.statMAG(data.armor + 256)!=0)
		{
			title.append(tr(" Mag:+%1").arg(QString::number(Items.statMAG(data.armor + 256))));
		}
		if(Items.statLCK(data.armor + 256)!=0)
		{
			title.append(tr(" Lck:+%1").arg(QString::number(Items.statLCK(data.armor + 256))));
		}
		if(Items.statHP(data.armor + 256)!=0)
		{
			title.append(tr(" Hp:+%1%").arg(QString::number(Items.statHP(data.armor + 256))));
		}
		if(Items.statMP(data.armor + 256)!=0)
		{
			title.append(tr(" Mp:+%1%").arg(QString::number(Items.statMP(data.armor + 256))));
		}
		lblArmorStats->setText(title);
		//Accessory
		if(data.accessory <32)
		{
			str_bonus += Items.statSTR(data.accessory + 288);
			vit_bonus +=Items.statVIT(data.accessory + 288);
			dex_bonus +=Items.statDEX(data.accessory + 288);
			spi_bonus +=Items.statSPI(data.accessory + 288);
			mag_bonus +=Items.statMAG(data.accessory + 288);
			lck_bonus +=Items.statLCK(data.accessory+ 288);
			hp_bonus+=Items.statHP(data.accessory + 288);
			mp_bonus+=Items.statMP(data.accessory + 288);
			title.clear();
			title.append(tr("Accessory"));
			if(Items.statSTR(data.accessory + 288) != 0)
			{
				title.append(tr(" Str:+%1").arg(QString::number(Items.statSTR(data.accessory + 288))));
			}
			if(Items.statVIT(data.accessory + 288) !=0)
			{
				title.append(tr(" Vit:+%1").arg(QString::number(Items.statVIT(data.accessory + 288))));
			}
			if(Items.statDEX(data.accessory + 288)!=0)
			{
				title.append(tr(" Dex:+%1").arg(QString::number(Items.statDEX(data.accessory + 288))));
			}
			if(Items.statSPI(data.accessory + 288)!=0)
			{
				title.append(tr(" Spi:+%1").arg(QString::number(Items.statSPI(data.accessory + 288))));
			}
			if(Items.statMAG(data.accessory + 288)!=0)
			{
				title.append(tr(" Mag:+%1").arg(QString::number(Items.statMAG(data.accessory + 288))));
			}
			if(Items.statLCK(data.accessory + 288)!=0)
			{
				title.append(tr(" Lck:+%1").arg(QString::number(Items.statLCK(data.accessory + 288))));
			}
			if(Items.statHP(data.accessory + 288)!=0)
			{
				title.append(tr(" Hp:+%1%").arg(QString::number(Items.statHP(data.accessory + 288))));
			}
			if(Items.statMP(data.accessory + 288)!=0)
			{
				title.append(tr(" Mp:+%1%").arg(QString::number(Items.statMP(data.accessory + 288))));
			}
			accessory_box->setTitle(title);
		}
		else{title.clear();title.append(tr("Accessory"));accessory_box->setTitle(title);}
		//process materia
		for(int i=0;i<16;i++)
		{
			if(data.materias[i].id!=FF7Materia::EmptyId)
			{
				bool add=true;
				int level=0;
				int aptemp = Materias.ap2num(data.materias[i].ap);
				for(int m=0; m<Materias.levels(data.materias[i].id);m++){if(aptemp >= Materias.ap(data.materias[i].id,m)){level++;}}

				switch(i)
				{
					case 0: if(weapon_slot_1->isHidden()){add=false;};break;
					case 1: if(weapon_slot_2->isHidden()){add=false;}break;
					case 2: if(weapon_slot_3->isHidden()){add=false;}break;
					case 3: if(weapon_slot_4->isHidden()){add=false;}break;
					case 4: if(weapon_slot_5->isHidden()){add=false;}break;
					case 5: if(weapon_slot_6->isHidden()){add=false;}break;
					case 6: if(weapon_slot_7->isHidden()){add=false;}break;
					case 7: if(weapon_slot_8->isHidden()){add=false;}break;
					case 8: if(armor_slot_1->isHidden()){add=false;}break;
					case 9: if(armor_slot_2->isHidden()){add=false;}break;
					case 10:if(armor_slot_3->isHidden()){add=false;}break;
					case 11:if(armor_slot_4->isHidden()){add=false;}break;
					case 12:if(armor_slot_5->isHidden()){add=false;}break;
					case 13:if(armor_slot_6->isHidden()){add=false;}break;
					case 14:if(armor_slot_7->isHidden()){add=false;}break;
					case 15:if(armor_slot_8->isHidden()){add=false;}break;
				}
				if(add)
				{
					// no special materia that affects these stats.
					str_bonus +=Materias.statSTR(data.materias[i].id);
					vit_bonus +=Materias.statVIT(data.materias[i].id);
					spi_bonus +=Materias.statSPI(data.materias[i].id);
					//Show in Percentage.
                    if(data.materias[i].id == 0x00){mp_bonus +=(10*level);}                             else{mp_bonus+=Materias.statMP(data.materias[i].id);}
                    if(data.materias[i].id == 0x01){hp_bonus += (10*level);}                            else{hp_bonus+=Materias.statHP(data.materias[i].id);}
					//show exact numbers
					if(data.materias[i].id == 0x02){dex_bonus += data.dexterity * (0.01*(level*10));}   else{dex_bonus +=Materias.statDEX(data.materias[i].id);}
                    if(data.materias[i].id ==0x03){ mag_bonus += data.magic * (0.01*(level*10));}       else{mag_bonus +=Materias.statMAG(data.materias[i].id);}
                    if(data.materias[i].id ==0x04){ lck_bonus += data.luck * (0.01*(level*10));}        else{lck_bonus +=Materias.statLCK(data.materias[i].id);}
				 }// end of add case.
			}
		}

	}

	lbl_str_mat_bonus->setText(QString::number(str_bonus));
	lbl_vit_mat_bonus->setText(QString::number(vit_bonus));
	lbl_dex_mat_bonus->setText(QString::number(dex_bonus));
	lbl_spi_mat_bonus->setText(QString::number(spi_bonus));
	lbl_mag_mat_bonus->setText(QString::number(mag_bonus));
	lbl_lck_mat_bonus->setText(QString::number(lck_bonus));

	str_total+=str_bonus;
	vit_total+= vit_bonus;
	dex_total+= dex_bonus;
	spi_total+= spi_bonus;
	mag_total+= mag_bonus;
	lck_total+= lck_bonus;

	if(str_total < 256)lbl_str_total->setText(QString::number(str_total));
	else{lbl_str_total->setText(QString::number(255));}

	if(vit_total  < 256)lbl_vit_total->setText(QString::number(vit_total));
	else{lbl_vit_total->setText(QString::number(255));}

	if(dex_total < 256)lbl_dex_total->setText(QString::number(dex_total));
	else{lbl_dex_total->setText(QString::number(255));}

	if(spi_total < 256)lbl_spi_total->setText(QString::number(spi_total));
	else{lbl_spi_total->setText(QString::number(255));}

	if(mag_total < 256)lbl_mag_total->setText(QString::number(mag_total));
	else{lbl_mag_total->setText(QString::number(255));}

	if(lck_total < 256)lbl_lck_total->setText(QString::number(lck_total));
	else{lbl_lck_total->setText(QString::number(255));}

	if(hp_bonus>0){lbl_base_hp_bonus->setText(QString(" +%2 (+%1%)").arg(QString::number(hp_bonus),QString::number(int(data.baseHP *(hp_bonus*0.01)))));}
	else if(hp_bonus<0){lbl_base_hp_bonus->setText(QString(" %2 (%1%)").arg(QString::number(hp_bonus),QString::number(int(data.baseHP *(hp_bonus*0.01)))));}
	else{lbl_base_hp_bonus->setText(QString(""));}

	if(mp_bonus>0){lbl_base_mp_bonus->setText(QString(" +%2 (+%1%)").arg(QString::number(mp_bonus),QString::number(int(data.baseMP *(mp_bonus*0.01)))));}
	else if(mp_bonus<0){lbl_base_mp_bonus->setText(QString(" %2 (%1%)").arg(QString::number(mp_bonus),QString::number(int(data.baseMP *(mp_bonus*0.01)))));}
	else{lbl_base_mp_bonus->setText(QString(""));}

	sb_maxHp->setValue(data.baseHP + (data.baseHP * (hp_bonus*.01)));
	lbl_hp_max->setText(QString(" %1").arg(QString::number(sb_maxHp->value())));
	sb_maxMp->setValue(data.baseMP + (data.baseMP *(mp_bonus *.01)));
	lbl_mp_max->setText(QString(" %1").arg(QString::number(sb_maxMp->value())));

}

void CharEditor::level_up(int pre_level)
{
	if(pre_level < data.level)
	{//level up
		for(int i=pre_level;i<data.level;i++)
		{// for statGain stat guide, 0=str; 1=vit;2=mag;3=spr;4=dex;5=lck;6=basehp;7basemp also use id incase of mods that could move a char.
			sb_str->setValue(data.strength + Chars.statGain(data.id,0,data.strength,i,i+1));
			sb_vit->setValue(data.vitality + Chars.statGain(data.id,1,data.vitality,i,i+1));
			sb_mag->setValue(data.magic + Chars.statGain(data.id,2,data.magic,i,i+1));
			sb_spi->setValue(data.spirit + Chars.statGain(data.id,3,data.spirit,i,i+1));
			sb_dex->setValue(data.dexterity + Chars.statGain(data.id,4,data.dexterity,i,i+1));
			sb_lck->setValue(data.luck + Chars.statGain(data.id,5,data.luck,i,i+1));
			sb_base_hp->setValue(data.baseHP + Chars.statGain(data.id,6,data.baseHP,i,i+1));
			sb_base_mp->setValue(data.baseMP + Chars.statGain(data.id,7,data.baseMP,i,i+1));
		 }
	}
	else if(pre_level > data.level)
	{//level down
		for(int i=pre_level;i>data.level;i--)
		{// for statGain stat guide, 0=str; 1=vit; 2=mag; 3=spr; 4=dex; 5=lck; 6=basehp; 7basemp
			sb_str->setValue(data.strength - Chars.statGain(data.id,0,data.strength,i,i-1));
			sb_vit->setValue(data.vitality - Chars.statGain(data.id,1,data.vitality,i,i-1));
			sb_mag->setValue(data.magic - Chars.statGain(data.id,2,data.magic,i,i-1));
			sb_spi->setValue(data.spirit - Chars.statGain(data.id,3,data.spirit,i,i-1));
			sb_dex->setValue(data.dexterity - Chars.statGain(data.id,4,data.dexterity,i,i-1));
			sb_lck->setValue(data.luck - Chars.statGain(data.id,5,data.luck,i,i-1));
			sb_base_hp->setValue(data.baseHP - Chars.statGain(data.id,6,data.baseHP,i,i-1));
			sb_base_mp->setValue(data.baseMP - Chars.statGain(data.id,7,data.baseMP,i,i-1));
		}
	}
	calc_stats();
}
void CharEditor::update_tnl_bar(void)
{
	if(data.level!=99)
	{
		setExpNext(Chars.totalExpForLevel(data.id,data.level)- data.exp);
		if(data.level>0)
		{
			 setLevelProgress(((Chars.tnlForLevel(data.id,data.level)-data.expNext)*62)/Chars.tnlForLevel(data.id,data.level));
		}
	}
	else
	{
		setExpNext(0);
		setLevelProgress(0x3D);
	}
	bar_tnl->setValue(data.flags[2]);
	if(bar_tnl->value()<4){bar_tnl->setValue(0);}//ff7 ingores the value if its <4 (but we don't save this)
	lbl_level_next->setText(tr("Next: %1").arg(QString::number(data.expNext)));
}

void CharEditor::elemental_info(void)
{
	int y= elemental_effects->contentsMargins().top() +elemental_effects->contentsMargins().bottom();
	bool show=false;
	int item_id = 0;
	QString m_effect;
	elemental_effects->clear();
	for(int r=0;r<3;r++)
	{
		switch (r)
		{
			case 0:item_id = data.weapon +128; break;
			case 1:item_id = data.armor +256; break;
			case 2:item_id = data.accessory +288; break;
		 }
		if(item_id <0 || item_id >319){}
		else
		{
			for(int i=0;i<14;i++)
			{
				QString effect;
				int element=0;
				switch(i)
				{
					case 0: element=Items.elementRestoration(item_id); effect.append(tr("Restoration"));break;
					case 1: element=Items.elementFire(item_id); effect.append(tr("Fire")); break;
					case 2: element=Items.elementCold(item_id); effect.append(tr("Cold")); break;
					case 3: element=Items.elementLightning(item_id); effect.append(tr("Lightning")); break;
					case 4: element=Items.elementEarth(item_id); effect.append(tr("Earth")); break;
					case 5: element=Items.elementWind(item_id); effect.append(tr("Wind")); break;
					case 6: element=Items.elementWater(item_id); effect.append(tr("Water")); break;
					case 7: element=Items.elementGravity(item_id); effect.append(tr("Gravity")); break;
					case 8: element=Items.elementHoly(item_id); effect.append(tr("Holy")); break;
					case 9: element=Items.elementPoison(item_id); effect.append(tr("Poison")); break;
					case 10: element=Items.elementCut(item_id); effect.append(tr("Cut")); break;
					case 11: element=Items.elementShoot(item_id); effect.append(tr("Shoot")); break;
					case 12: element=Items.elementPunch(item_id); effect.append(tr("Punch")); break;
					case 13: element=Items.elementHit(item_id); effect.append(tr("Hit")); break;
				}
				switch(element)
				{
					case -3: effect.prepend(tr("Absorb:"));break;
					case -2: effect.prepend(tr("Nullify:"));break;
					case -1: effect.prepend(tr("Halve:"));break;
					case  0: effect.clear();break;
					case +1: effect.prepend(tr("Attack:"));break;
				}
				if(!effect.isNull() && !m_effect.contains(effect,Qt::CaseSensitive))
				{
						m_effect.append(effect);
						elemental_effects->addItem(effect);
						show=true; y+=elemental_effects->sizeHintForRow(0);
				}
			 }//end of for Loop
            elemental_effects->setFixedHeight(y);
			elemental_box->setFixedSize(205*scale,y +elemental_box->contentsMargins().top()+elemental_box->contentsMargins().bottom());
		}//end of else.
	}//end of other loop.
   elemental_box->setVisible(show);
   elemental_box->adjustSize();
}//end of function

void CharEditor::status_info(void)
{
	int y=status_effects->contentsMargins().top()+status_effects->contentsMargins().bottom();
	bool show=false;
	int item_id =0;
	QString m_status;
	status_effects->clear();
	status_effects->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	for(int r=0;r<3;r++)
	{
		switch (r)
		{
			case 0:item_id = data.weapon +128; break;
			case 1:item_id = data.armor +256; break;
			case 2:item_id = data.accessory +288; break;
		 }
		 if(item_id <0 || item_id >319){}
		 else
		 {
			for(int i=0;i<24;i++)
			{
				QString effect;
				int status=0;
				switch(i)
				{
					case 0: status=Items.statusDeath(item_id); effect.append(tr("Death"));break;
					case 1: status=Items.statusSlowNumb(item_id); effect.append(tr("Slow-Numb"));break;
					case 2: status=Items.statusDeathSentence(item_id); effect.append(tr("D.Sentence"));break;
					case 3: status=Items.statusParalysis(item_id); effect.append(tr("Paralysis"));break;
					case 4: status=Items.statusPetrify(item_id); effect.append(tr("Petrify"));break;
					case 5: status=Items.statusSilence(item_id); effect.append(tr("Silence"));break;
					case 6: status=Items.statusSleep(item_id); effect.append(tr("Sleep"));break;
					case 7: status=Items.statusConfusion(item_id); effect.append(tr("Confusion"));break;
					case 8: status=Items.statusBerserk(item_id); effect.append(tr("Berserk"));break;
					case 9: status=Items.statusFrog(item_id); effect.append(tr("Frog"));break;
					case 10: status=Items.statusMini(item_id); effect.append(tr("Mini"));break;
					case 11: status=Items.statusPoison(item_id); effect.append(tr("Poison"));break;
					case 12: status=Items.statusFury(item_id); effect.append(tr("Fury"));break;
					case 13: status=Items.statusSadness(item_id); effect.append(tr("Sadness"));break;
					case 14: status=Items.statusDarkness(item_id); effect.append(tr("Darkness"));break;
					case 15: status=Items.statusHaste(item_id); effect.append(tr("Haste"));break;
					case 16: status=Items.statusSlow(item_id); effect.append(tr("Slow"));break;
					case 17: status=Items.statusStop(item_id); effect.append(tr("Stop"));break;
					case 18: status=Items.statusBarrier(item_id); effect.append(tr("Barrier"));break;
					case 19: status=Items.statusMagicBarrier(item_id); effect.append(tr("M.Barrier"));break;
					case 20: status=Items.statusReflect(item_id); effect.append(tr("Reflect"));break;
					case 21: status=Items.statusShield(item_id); effect.append(tr("Shield"));break;
					case 22: status=Items.statusRegen(item_id); effect.append(tr("Regen"));break;
			case 23: status=Items.statusResist(item_id); effect.append(tr("Regen"));break;
				}
				switch(status)
				{
					case -2: effect.prepend(tr("Protect:")); break;
					case -1: effect.prepend(tr("Remove:")); break;
					case  0: effect.clear();break;
					case +1: effect.prepend(tr("Inflict:")); break;
					case +2: effect.prepend(tr("OnBattle:"));break;
				}
				if(!effect.isNull() && !m_status.contains(effect,Qt::CaseSensitive))
				{
					m_status.append(effect);
					status_effects->addItem(effect);
					show=true; y+=status_effects->sizeHintForRow(0);
				}
			}//end of for Loop
            status_effects->setFixedHeight(y);
			status_box->setFixedSize(205*scale,y + status_box->contentsMargins().top()+ status_box->contentsMargins().bottom());
		}//end of else.
	 }//end of loop
	status_box->setVisible(show);
	status_box->adjustSize();
}//end of function
void CharEditor::update_materia_slots()
{
	weapon_slot_8->setHidden(1);
	weapon_slot_7->setHidden(1);
	weapon_slot_6->setHidden(1);
	weapon_slot_5->setHidden(1);
	weapon_slot_4->setHidden(1);
	weapon_slot_3->setHidden(1);
	weapon_slot_2->setHidden(1);
	weapon_slot_1->setHidden(1);
	armor_slot_8->setHidden(1);
	armor_slot_7->setHidden(1);
	armor_slot_6->setHidden(1);
	armor_slot_5->setHidden(1);
	armor_slot_4->setHidden(1);
	armor_slot_3->setHidden(1);
	armor_slot_2->setHidden(1);
	armor_slot_1->setHidden(1);

	//reset style sheet for the outer slot frames and links to ensure they are not visible reguardless of hosts stylesheet.
	armor_frm_1->setStyleSheet(QString("QFrame{background-color:rgba(0,0,0,0);}"));
	armor_frm_2->setStyleSheet(QString("QFrame{background-color:rgba(0,0,0,0);}"));
	armor_frm_3->setStyleSheet(QString("QFrame{background-color:rgba(0,0,0,0);}"));
	armor_frm_4->setStyleSheet(QString("QFrame{background-color:rgba(0,0,0,0);}"));
	armor_frm_5->setStyleSheet(QString("QFrame{background-color:rgba(0,0,0,0);}"));
	armor_frm_6->setStyleSheet(QString("QFrame{background-color:rgba(0,0,0,0);}"));
	armor_frm_7->setStyleSheet(QString("QFrame{background-color:rgba(0,0,0,0);}"));
	armor_frm_8->setStyleSheet(QString("QFrame{background-color:rgba(0,0,0,0);}"));
	weapon_frm_8->setStyleSheet(QString("QFrame{background-color:rgba(0,0,0,0);}"));
	weapon_frm_7->setStyleSheet(QString("QFrame{background-color:rgba(0,0,0,0);}"));
	weapon_frm_6->setStyleSheet(QString("QFrame{background-color:rgba(0,0,0,0);}"));
	weapon_frm_5->setStyleSheet(QString("QFrame{background-color:rgba(0,0,0,0);}"));
	weapon_frm_4->setStyleSheet(QString("QFrame{background-color:rgba(0,0,0,0);}"));
	weapon_frm_3->setStyleSheet(QString("QFrame{background-color:rgba(0,0,0,0);}"));
	weapon_frm_2->setStyleSheet(QString("QFrame{background-color:rgba(0,0,0,0);}"));
	weapon_frm_1->setStyleSheet(QString("QFrame{background-color:rgba(0,0,0,0);}"));

	weapon_m_link_1->setStyleSheet(QString("background-color:rgba(0,0,0,0);"));
	weapon_m_link_2->setStyleSheet(QString("background-color:rgba(0,0,0,0);"));
	weapon_m_link_3->setStyleSheet(QString("background-color:rgba(0,0,0,0);"));
	weapon_m_link_4->setStyleSheet(QString("background-color:rgba(0,0,0,0);"));
	armor_m_link_1->setStyleSheet(QString("background-color:rgba(0,0,0,0);"));
	armor_m_link_2->setStyleSheet(QString("background-color:rgba(0,0,0,0);"));
	armor_m_link_3->setStyleSheet(QString("background-color:rgba(0,0,0,0);"));
	armor_m_link_4->setStyleSheet(QString("background-color:rgba(0,0,0,0);"));

	 //fill the slots.
    QSize isize = QSize(int(24*scale), int(24*scale));
    if(data.materias[0].id!=FF7Materia::EmptyId){weapon_slot_1->setIcon(QIcon(Materias.pixmap(data.materias[0].id).scaled(isize,Qt::KeepAspectRatio,Qt::SmoothTransformation)));}else{weapon_slot_1->setIcon(QIcon(QString("")));}
    if(data.materias[0].id!=FF7Materia::EmptyId){weapon_slot_1->setToolTip(Materias.name(data.materias[0].id));}else{weapon_slot_1->setToolTip(QString(tr("-Empty-")));}
    if(data.materias[1].id!=FF7Materia::EmptyId){weapon_slot_2->setIcon(QIcon(Materias.pixmap(data.materias[1].id).scaled(isize,Qt::KeepAspectRatio,Qt::SmoothTransformation)));}else{weapon_slot_2->setIcon(QIcon(QString("")));}
    if(data.materias[1].id!=FF7Materia::EmptyId){weapon_slot_2->setToolTip(Materias.name(data.materias[1].id));}else{weapon_slot_2->setToolTip(QString(tr("-Empty-")));}
    if(data.materias[2].id!=FF7Materia::EmptyId){weapon_slot_3->setIcon(QIcon(Materias.pixmap(data.materias[2].id).scaled(isize,Qt::KeepAspectRatio,Qt::SmoothTransformation)));}else{weapon_slot_3->setIcon(QIcon(QString("")));}
    if(data.materias[2].id!=FF7Materia::EmptyId){weapon_slot_3->setToolTip(Materias.name(data.materias[2].id));}else{weapon_slot_3->setToolTip(QString(tr("-Empty-")));}
    if(data.materias[3].id!=FF7Materia::EmptyId){weapon_slot_4->setIcon(QIcon(Materias.pixmap(data.materias[3].id).scaled(isize,Qt::KeepAspectRatio,Qt::SmoothTransformation)));}else{weapon_slot_4->setIcon(QIcon(QString("")));}
    if(data.materias[3].id!=FF7Materia::EmptyId){weapon_slot_4->setToolTip(Materias.name(data.materias[3].id));}else{weapon_slot_4->setToolTip(QString(tr("-Empty-")));}
    if(data.materias[4].id!=FF7Materia::EmptyId){weapon_slot_5->setIcon(QIcon(Materias.pixmap(data.materias[4].id).scaled(isize,Qt::KeepAspectRatio,Qt::SmoothTransformation)));}else{weapon_slot_5->setIcon(QIcon(QString("")));}
    if(data.materias[4].id!=FF7Materia::EmptyId){weapon_slot_5->setToolTip(Materias.name(data.materias[4].id));}else{weapon_slot_5->setToolTip(QString(tr("-Empty-")));}
    if(data.materias[5].id!=FF7Materia::EmptyId){weapon_slot_6->setIcon(QIcon(Materias.pixmap(data.materias[5].id).scaled(isize,Qt::KeepAspectRatio,Qt::SmoothTransformation)));}else{weapon_slot_6->setIcon(QIcon(QString("")));}
    if(data.materias[5].id!=FF7Materia::EmptyId){weapon_slot_6->setToolTip(Materias.name(data.materias[5].id));}else{weapon_slot_6->setToolTip(QString(tr("-Empty-")));}
    if(data.materias[6].id!=FF7Materia::EmptyId){weapon_slot_7->setIcon(QIcon(Materias.pixmap(data.materias[6].id).scaled(isize,Qt::KeepAspectRatio,Qt::SmoothTransformation)));}else{weapon_slot_7->setIcon(QIcon(QString("")));}
    if(data.materias[6].id!=FF7Materia::EmptyId){weapon_slot_7->setToolTip(Materias.name(data.materias[6].id));}else{weapon_slot_7->setToolTip(QString(tr("-Empty-")));}
    if(data.materias[7].id!=FF7Materia::EmptyId){weapon_slot_8->setIcon(QIcon(Materias.pixmap(data.materias[7].id).scaled(isize,Qt::KeepAspectRatio,Qt::SmoothTransformation)));}else{weapon_slot_8->setIcon(QIcon(QString("")));}
    if(data.materias[7].id!=FF7Materia::EmptyId){weapon_slot_8->setToolTip(Materias.name(data.materias[7].id));}else{weapon_slot_8->setToolTip(QString(tr("-Empty-")));}
    if(data.materias[8].id!=FF7Materia::EmptyId){armor_slot_1->setIcon(QIcon(Materias.pixmap(data.materias[8].id).scaled(isize,Qt::KeepAspectRatio,Qt::SmoothTransformation)));}else{armor_slot_1->setIcon(QIcon(QString("")));}
    if(data.materias[8].id!=FF7Materia::EmptyId){armor_slot_1->setToolTip(Materias.name(data.materias[8].id));}else{armor_slot_1->setToolTip(QString(tr("-Empty-")));}
    if(data.materias[9].id!=FF7Materia::EmptyId){armor_slot_2->setIcon(QIcon(Materias.pixmap(data.materias[9].id).scaled(isize,Qt::KeepAspectRatio,Qt::SmoothTransformation)));}else{armor_slot_2->setIcon(QIcon(QString("")));}
    if(data.materias[9].id!=FF7Materia::EmptyId){armor_slot_2->setToolTip(Materias.name(data.materias[9].id));}else{armor_slot_2->setToolTip(QString(tr("-Empty-")));}
    if(data.materias[10].id!=FF7Materia::EmptyId){armor_slot_3->setIcon(QIcon(Materias.pixmap(data.materias[10].id).scaled(isize,Qt::KeepAspectRatio,Qt::SmoothTransformation)));}else{armor_slot_3->setIcon(QIcon(QString("")));}
    if(data.materias[10].id!=FF7Materia::EmptyId){armor_slot_3->setToolTip(Materias.name(data.materias[10].id));}else{armor_slot_3->setToolTip(QString(tr("-Empty-")));}
    if(data.materias[11].id!=FF7Materia::EmptyId){armor_slot_4->setIcon(QIcon(Materias.pixmap(data.materias[11].id).scaled(isize,Qt::KeepAspectRatio,Qt::SmoothTransformation)));}else{armor_slot_4->setIcon(QIcon(QString("")));}
    if(data.materias[11].id!=FF7Materia::EmptyId){armor_slot_4->setToolTip(Materias.name(data.materias[11].id));}else{armor_slot_4->setToolTip(QString(tr("-Empty-")));}
    if(data.materias[12].id!=FF7Materia::EmptyId){armor_slot_5->setIcon(QIcon(Materias.pixmap(data.materias[12].id).scaled(isize,Qt::KeepAspectRatio,Qt::SmoothTransformation)));}else{armor_slot_5->setIcon(QIcon(QString("")));}
    if(data.materias[12].id!=FF7Materia::EmptyId){armor_slot_5->setToolTip(Materias.name(data.materias[12].id));}else{armor_slot_5->setToolTip(QString(tr("-Empty-")));}
    if(data.materias[13].id!=FF7Materia::EmptyId){armor_slot_6->setIcon(QIcon(Materias.pixmap(data.materias[13].id).scaled(isize,Qt::KeepAspectRatio,Qt::SmoothTransformation)));}else{armor_slot_6->setIcon(QIcon(QString("")));}
    if(data.materias[13].id!=FF7Materia::EmptyId){armor_slot_6->setToolTip(Materias.name(data.materias[13].id));}else{armor_slot_6->setToolTip(QString(tr("-Empty-")));}
    if(data.materias[14].id!=FF7Materia::EmptyId){armor_slot_7->setIcon(QIcon(Materias.pixmap(data.materias[14].id).scaled(isize,Qt::KeepAspectRatio,Qt::SmoothTransformation)));}else{armor_slot_7->setIcon(QIcon(QString("")));}
    if(data.materias[14].id!=FF7Materia::EmptyId){armor_slot_7->setToolTip(Materias.name(data.materias[14].id));}else{armor_slot_7->setToolTip(QString(tr("-Empty-")));}
    if(data.materias[15].id!=FF7Materia::EmptyId){armor_slot_8->setIcon(QIcon(Materias.pixmap(data.materias[15].id).scaled(isize,Qt::KeepAspectRatio,Qt::SmoothTransformation)));}else{armor_slot_8->setIcon(QIcon(QString("")));}
    if(data.materias[15].id!=FF7Materia::EmptyId){armor_slot_8->setToolTip(Materias.name(data.materias[15].id));}else{armor_slot_8->setToolTip(QString(tr("-Empty-")));}

	 //set up weapon
	 QString ap_rate =tr("AP:x%1").arg(Items.materiaGrowthRate(data.weapon +128));
	 lblWeaponStats->setText(ap_rate);
	 switch(Items.materiaSlots(data.weapon +128))
	 {
		case 8:weapon_slot_8->setHidden(0);
		case 7:weapon_slot_7->setHidden(0);
		case 6:weapon_slot_6->setHidden(0);
		case 5:weapon_slot_5->setHidden(0);
		case 4:weapon_slot_4->setHidden(0);
		case 3:weapon_slot_3->setHidden(0);
		case 2:weapon_slot_2->setHidden(0);
		case 1:weapon_slot_1->setHidden(0);
	 };
	 switch(Items.linkedSlots((data.weapon +128)))
	  {
		case 4: weapon_m_link_4->setStyleSheet(Items.styleMateriaLink());
		case 3: weapon_m_link_3->setStyleSheet(Items.styleMateriaLink());
		case 2: weapon_m_link_2->setStyleSheet(Items.styleMateriaLink());
		case 1: weapon_m_link_1->setStyleSheet(Items.styleMateriaLink());
	  };
	 //set up armor
	 ap_rate =tr("AP:x%1").arg(Items.materiaGrowthRate(data.armor+256));
	 lblArmorStats->setText(ap_rate);
	 switch(Items.materiaSlots(data.armor +256))
	 {
		case 8:armor_slot_8->setHidden(0);
		case 7:armor_slot_7->setHidden(0);
		case 6:armor_slot_6->setHidden(0);
		case 5:armor_slot_5->setHidden(0);
		case 4:armor_slot_4->setHidden(0);
		case 3:armor_slot_3->setHidden(0);
		case 2:armor_slot_2->setHidden(0);
		case 1:armor_slot_1->setHidden(0);
	 };
	 switch(Items.linkedSlots((data.armor +256)))
	 {
		case 4: armor_m_link_4->setStyleSheet(Items.styleMateriaLink());
		case 3: armor_m_link_3->setStyleSheet(Items.styleMateriaLink());
		case 2: armor_m_link_2->setStyleSheet(Items.styleMateriaLink());
		case 1: armor_m_link_1->setStyleSheet(Items.styleMateriaLink());
	  };
	 calc_stats();
}
void CharEditor::matId_changed(qint8 id)
{
	if(id>=0 &&id<91){data.materias[mslotsel].id = id;}
	else{data.materias[mslotsel].id = FF7Materia::EmptyId;}
	if(!load){emit(Materias_changed(data.materias[mslotsel]));}
	update_materia_slots();
	calc_stats();
}
void CharEditor::matAp_changed(qint32 ap)
{
	if(ap>=0 && ap<FF7Materia::MaxMateriaAp)
	{
		int a = (ap & 0xff);
		int b = (ap & 0xff00) >> 8;
		int c = (ap & 0xff0000) >> 16;
		data.materias[mslotsel].ap[0]=a;
		data.materias[mslotsel].ap[1]=b;
		data.materias[mslotsel].ap[2]=c;
	}
	else
	{
		data.materias[mslotsel].ap[0]=0xFF;
		data.materias[mslotsel].ap[1]=0xFF;
		data.materias[mslotsel].ap[2]=0xFF;
	}
	if(!load){emit(Materias_changed(data.materias[mslotsel]));}
	update_materia_slots();
}
void CharEditor::weapon_slot_1_clicked(void){mButtonPress(0);}
void CharEditor::weapon_slot_2_clicked(void){mButtonPress(1);}
void CharEditor::weapon_slot_3_clicked(void){mButtonPress(2);}
void CharEditor::weapon_slot_4_clicked(void){mButtonPress(3);}
void CharEditor::weapon_slot_5_clicked(void){mButtonPress(4);}
void CharEditor::weapon_slot_6_clicked(void){mButtonPress(5);}
void CharEditor::weapon_slot_7_clicked(void){mButtonPress(6);}
void CharEditor::weapon_slot_8_clicked(void){mButtonPress(7);}
void CharEditor::armor_slot_1_clicked(void){mButtonPress(8);}
void CharEditor::armor_slot_2_clicked(void){mButtonPress(9);}
void CharEditor::armor_slot_3_clicked(void){mButtonPress(10);}
void CharEditor::armor_slot_4_clicked(void){mButtonPress(11);}
void CharEditor::armor_slot_5_clicked(void){mButtonPress(12);}
void CharEditor::armor_slot_6_clicked(void){mButtonPress(13);}
void CharEditor::armor_slot_7_clicked(void){mButtonPress(14);}
void CharEditor::armor_slot_8_clicked(void){mButtonPress(15);}

void CharEditor::mButtonPress(int Mslot)
{
	load=true;
	if(Mslot<0){return;}
	if(Mslot != mslotsel)
	{
		mslotsel = Mslot;
		materia_edit->setMateria(char_materia(mslotsel).id,Materias.ap2num(char_materia(mslotsel).ap));
		setSlotFrame();
		emit(mslotChanged(mslotsel));
	}
   else{materia_edit->setMateria(char_materia(mslotsel).id,Materias.ap2num(char_materia(mslotsel).ap));}
   load=false;
}

void CharEditor::setSlotFrame(void)
{
	weapon_frm_1->setFrameShape(QFrame::NoFrame);
	weapon_frm_2->setFrameShape(QFrame::NoFrame);
	weapon_frm_3->setFrameShape(QFrame::NoFrame);
	weapon_frm_4->setFrameShape(QFrame::NoFrame);
	weapon_frm_5->setFrameShape(QFrame::NoFrame);
	weapon_frm_6->setFrameShape(QFrame::NoFrame);
	weapon_frm_7->setFrameShape(QFrame::NoFrame);
	weapon_frm_8->setFrameShape(QFrame::NoFrame);
	armor_frm_1->setFrameShape(QFrame::NoFrame);
	armor_frm_2->setFrameShape(QFrame::NoFrame);
	armor_frm_3->setFrameShape(QFrame::NoFrame);
	armor_frm_4->setFrameShape(QFrame::NoFrame);
	armor_frm_5->setFrameShape(QFrame::NoFrame);
	armor_frm_6->setFrameShape(QFrame::NoFrame);
	armor_frm_7->setFrameShape(QFrame::NoFrame);
	armor_frm_8->setFrameShape(QFrame::NoFrame);

	switch(mslotsel)
	{
		case 0: weapon_frm_1->setFrameShape(QFrame::Box);break;
		case 1: weapon_frm_2->setFrameShape(QFrame::Box);break;
		case 2: weapon_frm_3->setFrameShape(QFrame::Box);break;
		case 3: weapon_frm_4->setFrameShape(QFrame::Box);break;
		case 4: weapon_frm_5->setFrameShape(QFrame::Box);break;
		case 5: weapon_frm_6->setFrameShape(QFrame::Box);break;
		case 6: weapon_frm_7->setFrameShape(QFrame::Box);break;
		case 7: weapon_frm_8->setFrameShape(QFrame::Box);break;
		case 8:  armor_frm_1->setFrameShape(QFrame::Box);break;
		case 9:  armor_frm_2->setFrameShape(QFrame::Box);break;
		case 10: armor_frm_3->setFrameShape(QFrame::Box);break;
		case 11: armor_frm_4->setFrameShape(QFrame::Box);break;
		case 12: armor_frm_5->setFrameShape(QFrame::Box);break;
		case 13: armor_frm_6->setFrameShape(QFrame::Box);break;
		case 14: armor_frm_7->setFrameShape(QFrame::Box);break;
		case 15: armor_frm_8->setFrameShape(QFrame::Box);break;
		default: break;
	};
}
void CharEditor::cb_idChanger_toggled(bool checked)
{
	if(checked && data.id == FF7Char::CaitSith){combo_id->setCurrentIndex(FF7Char::YoungCloud);}
	if(checked && data.id == FF7Char::Vincent){combo_id->setCurrentIndex(FF7Char::Sephiroth);}
	if(!checked && data.id ==FF7Char::YoungCloud){combo_id->setCurrentIndex(FF7Char::CaitSith);}
	if(!checked && data.id ==FF7Char::Sephiroth){combo_id->setCurrentIndex(FF7Char::Vincent);}
}
void CharEditor::MaxStats()
{
	if(data.id ==FF7Char::YoungCloud || data.id == FF7Char::Sephiroth){return;}
	else
	{
		sb_level->setValue(99);
		sb_base_hp->setValue(10000); //set to 10k to prevent overflow
		sb_base_mp->setValue(10000); //set to 10k to prevent overflow 
		sb_str_bonus->setValue(255);
		sb_dex_bonus->setValue(255);
		sb_spi_bonus->setValue(255);
		sb_vit_bonus->setValue(255);
		sb_mag_bonus->setValue(255);
		sb_lck_bonus->setValue(255);
		sb_curHp->setValue(data.maxHP);
		sb_curMp->setValue(data.maxMP);

		//do limits.
		if(data.id ==FF7Char::CaitSith)
		{
			sb_limit_level->setValue(2);
			this->setLimits(0x09);
		}
		else if (data.id ==FF7Char::Vincent)
		{
			sb_limit_level->setValue(4);
			this->setLimits(0x249);
		}
		else
		{
			sb_limit_level->setValue(4);
			this->setLimits(0x2DB);
		}
	}
}
void CharEditor::MaxEquip()
{
	if(data.id ==FF7Char::YoungCloud || data.id ==FF7Char::Sephiroth){return;}
	else
	{
		//set up weapons/ armor
		weapon_selection->setCurrentIndex(Chars.numberOfWeapons(data.id)-1);
		armor_selection->setCurrentIndex(29);
		accessory_selection->setCurrentIndex(18);
		for(int i=15;i>=0;i--)
		{
			mslotsel = i;
			quint8 new_id = FF7Materia::EmptyId;
			switch(mslotsel)
			{
				case 6: new_id = FF7Materia::MegaAll; break;
				case 5: new_id = FF7Materia::LongRange; break;
				case 4: new_id = FF7Materia::PreEmptive; break;
				case 3: new_id = FF7Materia::MasterSummon; break;
				case 2: new_id = FF7Materia::EnemySkill; break;
				case 1: new_id = FF7Materia::MasterCommand; break;
				case 0: weapon_slot_1_clicked();new_id = FF7Materia::MasterMagic; break;
			}

			data.materias[mslotsel].id = new_id;
			data.materias[mslotsel].ap[0] = 0xFF;
			data.materias[mslotsel].ap[1] = 0xFF;
			data.materias[mslotsel].ap[2] = 0xFF;

			emit(mslotChanged(mslotsel));
			emit(Materias_changed(data.materias[mslotsel]));
		}
		setSlotFrame();
		update_materia_slots();
		cb_front_row->setCheckState(Qt::Unchecked);
	}
}
void CharEditor::setEditableComboBoxes(bool editable)
{
		weapon_selection->setEditable(editable);
		weapon_selection->setInsertPolicy(QComboBox::NoInsert);

		armor_selection->setEditable(editable);
		armor_selection->setInsertPolicy(QComboBox::NoInsert);

		accessory_selection->setEditable(editable);
		accessory_selection->setInsertPolicy(QComboBox::NoInsert);
		materia_edit->setEditableMateriaCombo(editable);
}
