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
    <h1>M304 - {$data.ID}号機設定</h1>
    <table class="table-body">
      <tbody>
	<tr><th rowspan="3">機台番号<br><span style="font-size: 32px;">{$data.ID}</span></th>
	  <th colspan="2">{$data.NODE_NAME}</th><th colspan="3">{$data.FARM_NAME}</th></tr>
	<tr><th>{$data.DHCP}</th><th>{$data.FIXED_IPADDRESS}</th>
	  <th>{$data.FIXED_NETMASK}</th><th>{$data.FIXED_GATEWAY}</th><th>{$data.DNS}</th></tr>
	<tr><th colspan="5">{$data.COMMENT}</th></tr>
      </tbody>
    </table>
    <!-- blk_ope -->
    <table class="table-body">
      <thead>
	<tr>
	  <th class="w73" rowspan="2">OPE ID</th>
	  <th rowspan="2">動作名称</th>
	  <th colspan="8">リレー状態</th>
	  <th rowspan="2">コメント</th>
	  <th rowspan="2">操作</th>
	</tr>
	<tr>
	  {for $rn=1 to 8}
	  <th>RLY{$rn}</th>
	  {/for}
	</tr>
      </thead>
      <tbody>
	<!-- 一覧表示 blk_ope -->
	{for $rn=1 to 10}
	<tr>
	    <td><input type=NUMBER class="borderless num60 right-align" style="font-size: 16pt;" name="OPE_ID" value="{$data.OPE_ID[$rn]}"></td>
	    <td><input type=TEXT class="borderless" name="OPE_NAME" value="{$data.OPE_NAME}"></td>
	    {foreach from=$data['RLYSTATUS'][$rn] item=rlyval key=rlykey}
	    <td>
	      <select name="RLY[{$rlykey}]">
		{html_options options=$rlystatus selected=$rlyval}
	      </select>
	    </td>
	    {/foreach}
	    <td><input type=TEXT class="borderless num100" name="COMMENT" value="{$row.COMMENT}"></td>
	      <input type=HIDDEN name="ID"  value="{$row.OPE_ID}">
	      <input type=HIDDEN name="TBL" value="blk_ope">
	</tr>
	{/for}
      </tbody>
    </table>
    <!-- blk_c -->
    <table class="table-body">
      <thead>
	<tr>
	  <th class="w73">PKG ID</th><th>比較ID</th><th>有効</th><th>ROOM/REGION/ORDER</th><th>寿命</th><th>CCMTYPE</th><th>名称</th><th>操作</th>
	</tr>
      </thead>
      <tbody>
	{foreach from=$data['BLK_C'] item=row}
	<tr>
	    {if $row.ID==0}
	    <td rowspan="10"><input type=NUMBER class="borderless pkg_id r_16" name="PKGID" value="{$row.PKG_ID}"></td>
	    {/if}
	    <td><input type=NUMBER class="borderless pkg_id r_12" name="ID[{$row.ID}]" value="{$row.ID}"></td>
	    <td>
	      {if $row.VALID==1}
	      {$checked="checked"}
	      {else}
	      {$checked=""}
	      {/if}
	      <input type=CHECKBOX class="borderless" name="VALID[{$row.ID}]" {$checked} value="{$row.VALID}"></td>
	    <td class="center-align">
	      <input type=NUMBER class="borderless num3c r_12" name="ROOM[{$row.ID}]"  min=0 max=127 step=1 value="{$row.ROOM}">/
	      <input type=NUMBER class="borderless num3c r_12" name="REGION[{$row.ID}]" min=0 max=127 step=1 value="{$row.REGION}">/
	      <input type=NUMBER class="borderless num5c r_12" name="ORD[{$row.ID}]" min=0 max=30000 value="{$row.ORD}">
	    </td>
	    <td>
	      <select name="LIFECOUNT[{$row.ID}]">
		{html_options options=$lifecount_sel selected="{$row.LIFECOUNT}"}
	      </select>
	    </td>
	    <td><input type=TEXT class="borderless" maxlength=22 name="CCMTYPE[{$row.ID}]" value="{$row.CCMTYPE}"></td>
	    <td><input type=TEXT class="borderless num200" name="NAME[{$row.ID}]" value="{$row.NAME}"></td>
	    {if $row.ID==0}
	    <td rowspan="10" valign="BOTTOM">
	      <input type=HIDDEN name="SEQ" value="{$row.SEQ}">
	      <input type=HIDDEN name="PKG_ID" value="{$row.PKG_ID}">
	      <input type=HIDDEN name="TBL" value="blk_c">
<!--	      <input type=SUBMIT name="EXEC" value="変更">
	      <input type=SUBMIT name="EXEC" value="削除"> -->
	    </td>
	    {/if}
	</tr>
	{/foreach}
      </tbody>
    </table>
    <!-- blk_b -->
    <table class="table-body">
      <thead>
	<tr>
	  <th>PKG_ID</t><th>ID</th><th>有効</th><th>開始時分</th><th>終了時分</th>
          <th>動作間隔(分)</th><th>動作時間(分)</th><th>リレー動作<br>定義ID</th>
	</tr>
      </thead>
      <tbody>
	{foreach from=$data['BLK_B'] item=row}
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
	      <input type=CHECKBOX class="borderless" name="VALID[{$row.ID}]" {$checked} value="{$row.VALID}"></td>
	    <td>
	      <input type=NUMBER class="borderless num3c" name="STHR[{$row.ID}]" min=0 max=23 step=1 value="{$row.STHR}">:
	      <input type=NUMBER class="borderless num3c" name="STMN[{$row.ID}]" min=0 max=59 step=1 value="{$row.STMN}">
	    </td>
	    <td>
	      <input type=NUMBER class="borderless num3c" name="EDHR[{$row.ID}]" min=0 max=23 step=1 value="{$row.EDHR}">:
	      <input type=NUMBER class="borderless num3c" name="EDMN[{$row.ID}]" min=0 max=59 step=1 value="{$row.EDMN}">
	    </td>
	    <td>
	      <input type=NUMBER class="borderless num3c" name="INMN[{$row.ID}]"  min=0 max=255 step=1 value="{$row.INMN}">
	    </td>
	    <td>
	      <input type=NUMBER class="borderless num3c" name="DUMN[{$row.ID}]"  min=0 max=255 step=1 value="{$row.DUMN}">
	    </td>
	    <td>
	      <input type=NUMBER class="borderless num3c" name="OPE_ID[{$row.ID}]"  min=0 max=255 step=1 value="{$row.OPE_ID}">
	    </td>
	</tr>
	{/foreach}
      </tbody>
</table>
  </body>
</html>
