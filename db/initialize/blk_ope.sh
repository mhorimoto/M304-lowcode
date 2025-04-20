#! /bin/sh
ID=$1
echo mariadb -u m304 -p m304 -e "insert into blk_ope (ope_id,pkg_id,m304id,rly) values (${ID}01,${ID},${ID},1),(${ID}01,${ID},${ID},2),(${ID}01,${ID},${ID},3),(${ID}01,${ID},${ID},4),(${ID}01,${ID},${ID},5),(${ID}01,${ID},${ID},6),(${ID}01,${ID},${ID},7),(${ID}01,${ID},${ID},8);"

