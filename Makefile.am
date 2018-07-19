include $(top_srcdir)/common.mk

bin_LTLIBRARYS=
lib_LIBRARIES=
lib_LTLIBRARIES=

lib_LTLIBRARIES+=libwebml.la
libwebml_la_SOURCES=CommonTools.h CommonTools.cpp VectorHelper.h VectorHelper.cpp
libwebml_la_SOURCES+=RegressionLib/BaseFeature.cpp RegressionLib/BiddingFeature.h RegressionLib/FeatureManager.cpp RegressionLib/LqyFeature.cpp RegressionLib/RegDataCenter.h RegressionLib/StatFeature.cpp RegressionLib/TermWeightFeature.cpp RegressionLib/BaseFeature.h RegressionLib/CrossFeature.cpp RegressionLib/FeatureManager.h RegressionLib/LqyFeature.h RegressionLib/RegressionCommon.cpp RegressionLib/StatFeature.h RegressionLib/TermWeightFeature.h RegressionLib/BehaviorFeature.cpp RegressionLib/CrossFeature.h RegressionLib/FeatureTable.h RegressionLib/RankFeature.cpp RegressionLib/RegressionCommon.h RegressionLib/TermFeature.cpp RegressionLib/TermWeight.h RegressionLib/BehaviorFeature.h RegressionLib/DataProcessor.cpp RegressionLib/HighlightFeature.cpp RegressionLib/RankFeature.h RegressionLib/Regression.cpp RegressionLib/TermFeature.h RegressionLib/Tree.cpp RegressionLib/BiddingFeature.cpp RegressionLib/DataProcessor.h RegressionLib/HighlightFeature.h RegressionLib/RegDataCenter.cpp RegressionLib/Regression.h RegressionLib/TermWeight.cpp RegressionLib/Tree.h RegressionLib/DocPropFeature.h RegressionLib/DocPropFeature.cpp RegressionLib/SiteFeature.h RegressionLib/SiteFeature.cpp RegressionLib/ClickFeature.h RegressionLib/ClickFeature.cpp
libwebml_la_LIBADD=-lz -lmd5 -lACE -lCommonSegmentor -lCoreSegmentor -lencoding -ldocid

AM_LDFLAGS+=-version-number 4:0:$(SVNVERSION)
INCLUDES+=

SUBDIRS=@PACKAGE_DEPENDENCIES@ Tools
export @MODULE_ROOT_VARS@
export BUILD_DEPENDENCIES

