#! /bin/sh
ID=$1
mariadb -h www.smart-agri.jp -p m304 -e "insert into blk_b (pkg_id,m304id,id) values (${ID},${ID},0),(${ID},${ID},1),(${ID},${ID},2),(${ID},${ID},3),(${ID},${ID},4),(${ID},${ID},5),(${ID},${ID},6),(${ID},${ID},7),(${ID},${ID},8),(${ID},${ID},9);"

