<!DOCTYPE html>
<html lang="ja">
  <head>
    <meta charset="utf-8">
    <title>M304 parameter compiler</title>
    <meta name="description" content="https://at.strix-inc.jp/8487/">
    <meta name="keywords" content="M304">
    <meta name="viewport" content="width=device-width,initial-scale=1.0">
    <meta name="format-detection" content="telephone=no,email=no,address=no">
    
    <!-- Import Google Fonts -->
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Noto+Sans+JP:wght@400;500;700&display=swap" rel="stylesheet">
    
    <!-- Import CSS -->
    <link rel="stylesheet" href="css/style.css">
    
    <!-- Import JS/jQuery Library -->
    <script src="https://code.jquery.com/jquery-1.12.4.min.js"></script>
  </head>
  <body>
    <table style="border: 0;">
      <tr>
	<th style="border:0;">
	  <h1 style="border:0;">M304 - {$data_a.M304ID}号機設定&emsp;</h1>
	</th>
	<td style="border:0;text-align:left;">
          <a href="file_operation.php?ID={$data_a.M304ID}">
	    <input type="button" name="FILEDIR" value="FILE操作"></a>
	    <b>ファイルの読み込みや削除を行います。</b><br>
	  <form method="POST" action="m304body2.php?ID={$data_a.M304ID}">
	    <input type="submit" name="FILESAVE" value="FILE保存">
	    <input type="text" name="FILENAME" list="filenamelist" maxlength="20" placeholder="ファイル名を入力">
	    <input type="text" name="COMMENT" size="50" placeholder="コメントを入力">
	    <input type="text" name="OWNER" value="" list="ownerlist" size="10" placeholder="担当名を入力">
            <datalist id="filenamelist">
              {foreach from=$save_dir item=save_dir_row}
              <option value="{$save_dir_row.FILENAME}"></option>
              {/foreach}
            </datalist>
	    <datalist id="ownerlist">
              {foreach from=$save_dir item=save_dir_row}
              <option value="{$save_dir_row.OWNER}"></option>
              {/foreach}
	    </datalist><br>
	    FILE保存には,ファイル名,コメント,担当者、全ての項目が必須です。
	  </form>
	</td>
      </tr>
    </table>
    <table class="table-body">
      <tbody>
        {if $data_a.DHCPFLAG==0}
        {$dhcpf = "固定IP"}
        {else}
        {$dhcpf = "DHCP"}
        {/if}
        <input type="hidden" name="MACADDR" value="{$data_a.MACADDR}">
        <tr><th rowspan="4">機台番号<br><span style="font-size: 32px;">{$data_a.M304ID}</span></th>
          <th colspan="2">{$data_a.NODE_NAME}</th><th colspan="3">{$data_custom.NAME}</th></tr>
        <tr><th>{$dhcpf}</th><th>IP:{$data_a.FIXED_IPADDRESS}</th>
          <th>NETMASK:{$data_a.FIXED_NETMASK}</th><th>GW:{$data_a.FIXED_DEFGW}</th><th>DNS:{$data_a.FIXED_DNS}</th></tr>
        <tr>
          <td colspan="5">MACADDR:{$data_a.MACADDR}</td>
        </tr>
        <tr><th colspan="5">{$data_a.COMMENT|default:'<br>'}</th></tr>
      </tbody>
    </table>

    <!-- blk_b -->

    <table class="table-body">
      <thead>
        <tr>
          <th rowspan="2">PKG_ID</th><th>ID</th><th>有効</th><th>開始時分</th><th>終了時分</th>
          <th>動作間隔(分)</th><th>動作時間(分)</th>
          <th><a href="m304ope.php?PKG_ID={$data_a.M304ID}">リレー動作 定義ID</a></th>
          <th rowspan="2">操作</th>
        </tr>
        <tr>
          <th colspan="6">比較数式</th><th><a href="m304blk_c.php?PKG_ID={$data_a.M304ID}">評価CCMTYPE編集</a></th>
        </tr>
      </thead>
      <tbody>
        {foreach from=$data_b item=row}
        <form action="m304body2.php?ID={$data_a.M304ID}" method="POST">
          <input type="HIDDEN" name="DATA" value="BLK_B">
          <input type="HIDDEN" name="PKG_ID" value="{$row.PKG_ID}">
          <input type="HIDDEN" name="SEQ" value="{$row.SEQ}">
          <tr>
            {if $row.ID==0}
            <td rowspan="{($blk_b_maxrows+1)*2}">
              <p class="pkg_id r_16">{$row.PKG_ID}</p>
              <a href="https://www.smart-agri.jp/~horimoto/m304/conv2ihex.php?PRG=BLK_B&PKG_ID={$row.PKG_ID}" target="_blank">
                <input type="button" name="BLK_B_BUTTON" value="IH変換">
              </a>
              <a href="https://www.smart-agri.jp/~horimoto/m304/conv2ihex.php?PRG=BLK_B&PKG_ID={$row.PKG_ID}&amp;OD=F" target="_blank">
                <input type="button" name="BLK_B_BUTTON" value="IH更新">
              </a>
            </td>
            <!-- <td rowspan="{$blk_b_maxrows}"><input type=NUMBER class="borderless pkg_id r_16" name="PKGID" value="{$row.PKG_ID}"></td> -->
            {/if}
            <td><input type=NUMBER class="borderless pkg_id r_12" name="ID" value="{$row.ID}"></td>
            <td>
              {if $row.VALID==1}
              {$checked="checked"}
              {else}
              {$checked=""}
              {/if}
              <input type=CHECKBOX class="borderless" name="VALID" {$checked}>
            </td>
            <td>
              <input type=NUMBER class="borderless num3c r_12" name="STHR" min=0 max=23 step=1 value="{$row.STHR}">:
              <input type=NUMBER class="borderless num3c r_12" name="STMN" min=0 max=59 step=1 value="{$row.STMN}">
            </td>
            <td>
              <input type=NUMBER class="borderless num3c r_12" name="EDHR" min=0 max=23 step=1 value="{$row.EDHR}">:
              <input type=NUMBER class="borderless num3c r_12" name="EDMN" min=0 max=59 step=1 value="{$row.EDMN}">
            </td>
            <td>
              <input type=NUMBER class="borderless num3c r_12" name="INMN"  min=0 max=255 step=1 value="{$row.INMN}">
            </td>
            <td>
              <input type=NUMBER class="borderless num3c r_12" name="DUMN"  min=0 max=255 step=1 value="{$row.DUMN}">
            </td>
            <td>
              <select name="OPE_ID">
                {html_options options=$ope_c_name_list selected="{$row.OPE_ID}"}
              </select>
            </td>
            <td rowspan="2">
              <nobr>
                <input type=SUBMIT name="cmd" style="height:44px;background-color: palegreen;" value="変更">
                <input type=SUBMIT name="cmd" style="height:44px;background-color: lightpink;" value="削除">
              </nobr>
            </td>
          </tr>
          <tr>
            <td colspan="7">
              <!-- -->
              <select name="CMPCCMID0">
                {html_options options=$select_ce selected="{$row.CMPCCMID0}"}
              </select>
              <select name="CMPOPE0">
                {html_options options=$select_rlyope selected="{$row.CMPOPE0}"}
              </select>
              <input type="NUMBER" name="CMPVAL0" value="{$row.CMPVAL0}" step=0.01 class="num60"}>
              <!-- 1  -->
              <select name="CMBCMP1">
                {html_options options=$select_cmbope selected="{$row.CMBCMP1}"}
              </select>
              <select name="CMPCCMID1">
                {html_options options=$select_ce selected="{$row.CMPCCMID1}"}
              </select>
              <select name="CMPOPE1">
                {html_options options=$select_rlyope selected="{$row.CMPOPE1}"}
              </select>
              <input type="NUMBER" name="CMPVAL1" value="{$row.CMPVAL1}" step=0.01 class="num60"}>
              <!-- 2 -->
              <select name="CMBCMP2">
                {html_options options=$select_cmbope selected="{$row.CMBCMP2}"}
              </select>
              <select name="CMPCCMID2">
                {html_options options=$select_ce selected="{$row.CMPCCMID2}"}
              </select>
              <select name="CMPOPE2">
                {html_options options=$select_rlyope selected="{$row.CMPOPE2}"}
              </select>
              <input type="NUMBER" name="CMPVAL2" value="{$row.CMPVAL2}" step=0.01 class="num60"}>
              <!-- 3 -->
              <select name="CMBCMP3">
                {html_options options=$select_cmbope selected="{$row.CMBCMP3}"}
              </select>
              <select name="CMPCCMID3">
                {html_options options=$select_ce selected="{$row.CMPCCMID3}"}
              </select>
              <select name="CMPOPE3">
                {html_options options=$select_rlyope selected="{$row.CMPOPE3}"}
              </select>
              <input type="NUMBER" name="CMPVAL3" value="{$row.CMPVAL3}" step=0.01 class="num60"}>
              <!-- 4 -->
              <select name="CMBCMP4">
                {html_options options=$select_cmbope selected="{$row.CMBCMP4}"}
              </select>
              <select name="CMPCCMID4">
                {html_options options=$select_ce selected="{$row.CMPCCMID4}"}
              </select>
              <select name="CMPOPE4">
                {html_options options=$select_rlyope selected="{$row.CMPOPE4}"}
              </select>
              <input type="NUMBER" name="CMPVAL4" value="{$row.CMPVAL4}" step=0.01 class="num60"}>
              <!-- 5 -->
            </td>
          </tr>
        </form>
        {/foreach}
        <!-- 新規追加の欄 -->
        <form action="m304body2.php?ID={$data_a.M304ID}" method="POST">
          {$nid = $row.ID+1}
          <input type="HIDDEN" name="DATA" value="BLK_B">
          <input type="HIDDEN" name="PKG_ID" value="{$row.PKG_ID}">
          <tr>
            {if $row.ID==0}
            <td rowspan="{$blk_b_maxrows}"><p class="pkg_id r_16">{$row.PKG_ID}</p></td>
            <!-- <td rowspan="{$blk_b_maxrows}"><input type=NUMBER class="borderless pkg_id r_16" name="PKGID" value="{$row.PKG_ID}"></td> -->
            {/if}
            <td><input type=NUMBER class="borderless pkg_id r_12" name="ID" value="{$nid}"></td>
            <td>
              <input type=CHECKBOX class="borderless" name="VALID" checked>
            </td>
            <td>
              <input type=NUMBER class="borderless num3c r_12" name="STHR" min=0 max=23 step=1 value="">:
              <input type=NUMBER class="borderless num3c r_12" name="STMN" min=0 max=59 step=1 value="">
            </td>
            <td>
              <input type=NUMBER class="borderless num3c r_12" name="EDHR" min=0 max=23 step=1 value="">:
              <input type=NUMBER class="borderless num3c r_12" name="EDMN" min=0 max=59 step=1 value="">
            </td>
            <td>
              <input type=NUMBER class="borderless num3c r_12" name="INMN"  min=0 max=255 step=1 value="">
            </td>
            <td>
              <input type=NUMBER class="borderless num3c r_12" name="DUMN"  min=0 max=255 step=1 value="">
            </td>
            <td>
              <select name="OPE_ID">
                {html_options options=$ope_c_name_list selected="{$row.OPE_ID}"}
              </select>
            </td>
            <td rowspan="2">
              <input type=SUBMIT name="cmd" value="新規追加">
            </td>
          </tr>
          <tr>
            <td colspan="7">
              <!-- -->
              <select name="CMPCCMID0">
                {html_options options=$select_ce}
              </select>
              <select name="CMPOPE0">
                {html_options options=$select_rlyope}
              </select>
              <input type="NUMBER" name="CMPVAL0" value="" step=0.01 class="num60"}>
              <!-- 1  -->
              <select name="CMBCMP1">
                {html_options options=$select_cmbope}
              </select>
              <select name="CMPCCMID1">
                {html_options options=$select_ce}
              </select>
              <select name="CMPOPE1">
                {html_options options=$select_rlyope}
              </select>
              <input type="NUMBER" name="CMPVAL1" value="" step=0.01 class="num60"}>
              <!-- 2 -->
              <select name="CMBCMP2">
                {html_options options=$select_cmbope}
              </select>
              <select name="CMPCCMID2">
                {html_options options=$select_ce}
              </select>
              <select name="CMPOPE2">
                {html_options options=$select_rlyope}
              </select>
              <input type="NUMBER" name="CMPVAL2" value="" step=0.01 class="num60"}>
              <!-- 3 -->
              <select name="CMBCMP3">
                {html_options options=$select_cmbope}
              </select>
              <select name="CMPCCMID3">
                {html_options options=$select_ce}
              </select>
              <select name="CMPOPE3">
                {html_options options=$select_rlyope}
              </select>
              <input type="NUMBER" name="CMPVAL3" value="" step=0.01 class="num60"}>
              <!-- 4 -->
              <select name="CMBCMP4">
                {html_options options=$select_cmbope selected="{$row.CMBCMP4}"}
              </select>
              <select name="CMPCCMID4">
                {html_options options=$select_ce selected="{$row.CMPCCMID4}"}
              </select>
              <select name="CMPOPE4">
                {html_options options=$select_rlyope selected="{$row.CMPOPE4}"}
              </select>
              <input type="NUMBER" name="CMPVAL4" value="" step=0.01 class="num60"}>
              <!-- 5 -->
            </td>
          </tr>
        </form>
      </tbody>
    </table>
    <table class="table-body">
      <thead>
        <tr>
          <th>PKG_ID</th><th>ID</th><th>有効</th><th>ROOM/REGION/ORDER/PRIORITY</th><th>LV</th>
          <th>CAST</th><th>単位</th><th>CCMTYPE</th><th>名前</th><th>操作</th>
        </tr>
      </thead>
      <tbody>
        {if $data_d_maxrows>0}
        {foreach from=$data_d item=row}
        <form action="m304body2.php?ID={$data_a.M304ID}" method="POST">
          <tr>
            {if $row@first}
            <td rowspan="{$data_d_maxrows+1}"><p class="pkg_id r_16">{$row.PKG_ID}</p>
              <a href="https://www.smart-agri.jp/~horimoto/m304/conv2ihex.php?PRG=BLK_D&PKG_ID={$row.PKG_ID}" target="_IHEXD">
                <input type="button" name="BLK_D_BUTTON" value="IH変換">
              </a>
              <a href="https://www.smart-agri.jp/~horimoto/m304/conv2ihex.php?PRG=BLK_D&PKG_ID={$row.PKG_ID}&amp;OD=F" target="_IHEXD">
                <input type="button" name="BLK_D_BUTTON" value="IH更新">
              </a>            </td>
            <!-- <td rowspan="{$data_d_maxrows}"><input type=NUMBER class="borderless pkg_id r_16" name="PKGID" value="{$row.PKG_ID}"></td> -->
            {/if}
            <input type="HIDDEN" name="PKGID" value="{$row.PKG_ID}"/>
            <input type="HIDDEN" name="DATA"  value="BLK_D"/>
            <td><input type=NUMBER class="borderless pkg_id r_12" name="ID" value="{$row.ID}"></td>
            <td>
              {if $row.VALID==1}
              {$checked="checked"}
              {else}
              {$checked=""}
              {/if}
              <input type=CHECKBOX class="borderless" name="VALID" {$checked}>
            </td>
            <td>
              <input type=NUMBER class="borderless num3c" name="ROOM"  min=0 max=127 step=1 value="{$row.ROOM}">/
              <input type=NUMBER class="borderless num3c" name="REGION" min=0 max=127 step=1 value="{$row.REGION}">/
              <input type=NUMBER class="borderless num5c" name="ORD" min=0 max=30000 value="{$row.ORD}">/
              <input type=NUMBER class="borderless num3c" name="PRIORITY"  min=0 max=30 step=1 value="{$row.PRIORITY}">
            </td>
            <td>
              <select name="LV">
                {html_options options=$lv_sel selected="{$row.LV}"}
              </select>
            </td>
            <td>
              <input type=NUMBER class="borderless num3c" name="CAST"  min=0 max=4 step=1 value="{$row.CAST}">
            </td>
            <td>
              <input type=TEXT class="borderless num60" name="UNIT" maxlength=10 value="{$row.UNIT}">
            </td>
            <td><input type=TEXT class="borderless w73" maxlength=22 name="CCMTYPE" value="{$row.CCMTYPE}"></td>
            <td><input type=TEXT class="borderless num200" name="NAME" value="{$row.NAME}"></td>
            <td>
              <nobr>
                <input type=SUBMIT name="cmd" style="background-color:palegreen;" value="変更">
                <input type=SUBMIT name="cmd" style="background-color:lightpink;" value="削除">
              </nobr>
            </td>  
          </tr>
        </form>
        {$nid = $row.ID+1}
        {$nroom = $row.ROOM}
        {$nregion = $row.REGION}
        {$nord = $row.ORD}
        {$nprio = $row.PRIORITY}
        {/foreach}
        {/if}
        <form action="m304body2.php?ID={$data_a.M304ID}" method="POST">
          <tr>
            {if $data_d_maxrows==0}
            <td rowspan="{$data_d_maxrows+1}"><p class="pkg_id r_16">{$row.PKG_ID}</p></td>
            {/if}
            <!-- <td rowspan="{$data_d_maxrows}"><input type=NUMBER class="borderless pkg_id r_16" name="PKGID" value="{$row.PKG_ID}"></td> -->
            <input type="HIDDEN" name="PKGID" value="{$row.PKG_ID}">
            <input type="HIDDEN" name="DATA"  value="BLK_D"/>
            <td><input type=NUMBER class="borderless pkg_id r_12" name="ID" value="{$nid}"></td>
            <td>
              {if $row.VALID==1}
              {$checked="checked"}
              {else}
              {$checked=""}
              {/if}
              <input type=CHECKBOX class="borderless" name="VALID" {$checked}>
            </td>
            <td>
              <input type=NUMBER class="borderless num3c" name="ROOM"  min=0 max=127 step=1 value="{$nroom}">/
              <input type=NUMBER class="borderless num3c" name="REGION" min=0 max=127 step=1 value="{$nregion}">/
              <input type=NUMBER class="borderless num5c" name="ORD" min=0 max=30000 value="{$nord}">/
              <input type=NUMBER class="borderless num3c" name="PRIORITY"  min=0 max=30 step=1 value="{$nprio}">
            </td>
            <td>
              <select name="LV">
                {html_options options=$lv_sel }
              </select>
            </td>
            <td>
              <input type=NUMBER class="borderless num3c" name="CAST"  min=0 max=4 step=1 value="0">
            </td>
            <td>
              <input type=TEXT class="borderless num60" name="UNIT" maxlength=10 placeholder="単位">
            </td>
            <td><input type=TEXT class="borderless w73" maxlength=22 name="CCMTYPE" placeholder="必須"></td>
            <td><input type=TEXT class="borderless num200" name="NAME" placeholder="なるべく入力"></td>
            <td><input type=SUBMIT name="cmd" value="新規追加"></td>  
          </tr>
        </form>
      </tbody>
    </table>
    <footer>
      <hr>
      <p>Ver: 2be</p>
    </footer>
  </body>
</html>
