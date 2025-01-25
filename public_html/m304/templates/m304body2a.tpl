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
    <h1>M304 - {$data_a.M304ID}号機設定</h1>
    <table class="table-body">
      <tbody>
	<tr><th rowspan="3">機台番号<br><span style="font-size: 32px;">{$data_a.M304ID}</span></th>
	  <th colspan="2">{$data_a.NODE_NAME}</th><th colspan="3">{$data_custom.NAME}</th></tr>
	<tr><th>{$data_a.DHCP}</th><th>{$data_a.FIXED_IPADDRESS}</th>
	  <th>{$data_a.FIXED_NETMASK}</th><th>{$data_a.FIXED_DEFGW}</th><th>{$data_a.FIXED_DNS}</th></tr>
	<tr><th colspan="5">{$data_a.COMMENT}</th></tr>
      </tbody>
    </table>
    <!-- blk_b -->
    <table class="table-body">
      <thead>
	<tr>
	  <th rowspan="2">PKG_ID</th><th>ID</th><th>有効</th><th>開始時分</th><th>終了時分</th>
	  <th>動作間隔(分)</th><th>動作時間(分)</th><th>リレー動作<br>定義ID</th><th rowspan="2">操作</th>
	</tr>
	<tr>
	  <th colspan="7">比較数式</td>
	</tr>
      </thead>
      <tbody>
	{foreach from=$data_b item=row}
	<tr>
	  {if $row.ID==0}
	  <td rowspan="{$blk_b_maxrows}"><input type=NUMBER class="borderless pkg_id r_16" name="PKGID" value="{$row.PKG_ID}"></td>
	  {/if}
	  <td><input type=NUMBER class="borderless pkg_id r_12" name="ID[{$row.ID}]" value="{$row.ID}"></td>
	  <td>
	    {if $row.VALID==1}
	      {$checked="checked"}
	    {else}
	      {$checked=""}
	    {/if}
	    <input type=CHECKBOX class="borderless" name="VALID[{$row.ID}]" {$checked} value="{$row.VALID}">
	  </td>
	  <td>
	    <input type=NUMBER class="borderless num3c r_12" name="STHR[{$row.ID}]" min=0 max=23 step=1 value="{$row.STHR}">:
	    <input type=NUMBER class="borderless num3c r_12" name="STMN[{$row.ID}]" min=0 max=59 step=1 value="{$row.STMN}">
	  </td>
	  <td>
	    <input type=NUMBER class="borderless num3c r_12" name="EDHR[{$row.ID}]" min=0 max=23 step=1 value="{$row.EDHR}">:
	    <input type=NUMBER class="borderless num3c r_12" name="EDMN[{$row.ID}]" min=0 max=59 step=1 value="{$row.EDMN}">
	  </td>
	  <td>
	    <input type=NUMBER class="borderless num3c r_12" name="INMN[{$row.ID}]"  min=0 max=255 step=1 value="{$row.INMN}">
	  </td>
	  <td>
	    <input type=NUMBER class="borderless num3c r_12" name="DUMN[{$row.ID}]"  min=0 max=255 step=1 value="{$row.DUMN}">
	  </td>
	  <td>
	    <select name="OPE_ID[{$row.ID}]">
	      {html_options options=$ope_c_name_list selected="{$row.OPE_ID}"}
	    </select>
	  </td>
	  <td rowspan="2"></td>
	</tr>
	<tr>
	  <td colspan="7">
	    <!-- -->
	    <select name="CMPCCMID0[{$row.ID}]">
	      {html_options options=$select_ce selected="{$row.CMPCCMID0}"}
	    </select>
	    <select name="CMPOPE0[{$row.ID}]">
	      {html_options options=$select_rlyope selected="{$row.CMPOPE0}"}
	    </select>
	    <input type="NUMBER" name="CMPVAL0[{$row.ID}]" value="{$row.CMPVAL0}" step=0.01 class="num60"}>
	    <!-- 1  -->
	    <select name="CMBOPE1[{$row.ID}]">
	      {html_options options=$select_cmbope selected="{$row.CMBOPE1}"}
	    </select>
	    <select name="CMPCCMID1[{$row.ID}]">
	      {html_options options=$select_ce selected="{$row.CMPCCMID1}"}
	    </select>
	    <select name="CMPOPE1[{$row.ID}]">
	      {html_options options=$select_rlyope selected="{$row.CMPOPE1}"}
	    </select>
	    <input type="NUMBER" name="CMPVAL1[{$row.ID}]" value="{$row.CMPVAL1}" step=0.01 class="num60"}>
	    <!-- 2 -->
	    <select name="CMBOPE2[{$row.ID}]">
	      {html_options options=$select_cmbope selected="{$row.CMBOPE2}"}
	    </select>
	    <select name="CMPCCMID2[{$row.ID}]">
	      {html_options options=$select_ce selected="{$row.CMPCCMID1}"}
	    </select>
	    <select name="CMPOPE2[{$row.ID}]">
	      {html_options options=$select_rlyope selected="{$row.CMPOPE1}"}
	    </select>
	    <input type="NUMBER" name="CMPVAL2[{$row.ID}]" value="{$row.CMPVAL2}" step=0.01 class="num60"}>
	    <!-- 3 -->
	    <select name="CMBOPE3[{$row.ID}]">
	      {html_options options=$select_cmbope selected="{$row.CMBOPE3}"}
	    </select>
	    <select name="CMPCCMID3[{$row.ID}]">
	      {html_options options=$select_ce selected="{$row.CMPCCMID3}"}
	    </select>
	    <select name="CMPOPE3[{$row.ID}]">
	      {html_options options=$select_rlyope selected="{$row.CMPOPE3}"}
	    </select>
	    <input type="NUMBER" name="CMPVAL3[{$row.ID}]" value="{$row.CMPVAL3}" step=0.01 class="num60"}>
	    <!-- 4 -->
	    <select name="CMBOPE4[{$row.ID}]">
	      {html_options options=$select_cmbope selected="{$row.CMBOPE4}"}
	    </select>
	    <select name="CMPCCMID4[{$row.ID}]">
	      {html_options options=$select_ce selected="{$row.CMPCCMID4}"}
	    </select>
	    <select name="CMPOPE4[{$row.ID}]">
	      {html_options options=$select_rlyope selected="{$row.CMPOPE4}"}
	    </select>
	    <input type="NUMBER" name="CMPVAL4[{$row.ID}]" value="{$row.CMPVAL4}" step=0.01 class="num60"}>
	    <!-- 5 -->
	  </td>
	</tr>
	{/foreach}
      </tbody>
    </table>
    <footer>
      <hr>
      <p>Ver: 2a</p>
    </footer>
  </body>
</html>
