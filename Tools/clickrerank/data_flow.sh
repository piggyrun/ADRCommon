#!/usr/bin/sh

for i in {4..30}
do
	#date=$1
	date=`date -d "$i day ago" +%Y%m%d`
	echo $date

	rsync la_user::root/DATA/odin/data/click_rerank/cpush/cut.log.$date.baidu.queryNum log/
	rsync la_user::root/DATA/odin/data/click_rerank/cpush/cut.log.$date.baidu.decode log/
	rsync la_user::root/DATA/odin/data/click_rerank/cpush/cut.log.$date.google.queryNum log/
	rsync la_user::root/DATA/odin/data/click_rerank/cpush/cut.log.$date.google.decode log/

	mkdir -p data/$date
	perl stat_cpush.pl log/cut.log.$date.baidu.queryNum log/cut.log.$date.baidu.decode data/$date/baidu_query_url
	perl stat_cpush.pl log/cut.log.$date.google.queryNum log/cut.log.$date.google.decode data/$date/google_query_url
done
