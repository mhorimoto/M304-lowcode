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
    <link href="https://fonts.googleapis.com/css2?family=Noto+Sans+JP:wght@400;500;700&display=swap" rel="
stylesheet">

    <!-- Import CSS -->
    <link rel="stylesheet" href="css/style.css">

    <!-- Import JS/jQuery Library -->
    <script src="https://code.jquery.com/jquery-1.12.4.min.js"></script>
  </head>
  <body>
    <h1>blk_b編集</h1>
    <p>{$msg}</p>
    {include file='link_list.inc'}
    {$maxrows2=$max_rows*2}
    <table>
      <thead>
	<tr>
	  <th>PKG_ID</t><th>ID</th><th>有効</th><th>開始時分</th><th>終了時分</th>
          <th>動作間隔(分)</th><th>動作時間(分)</th><th>リレー動作<br>定義ID</th>
	</tr>
      </thead>
      <tbody>
	<form action="manup.php" method="POST">
	{foreach from=$data item=row}
	<tr>
	    {if $row.ID==0}
	    <td rowspan="{$maxrows2}"><input type=NUMBER class="borderless pkg_id" name="PKGID" value="{$row.PKG_ID}"></td>
	    {/if}
	    <td><input type=NUMBER class="borderless pkg_id" name="ID[{$row.ID}]" value="{$row.ID}"></td>
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
	    {if $row.ID==0}
	    <td rowspan="10" valign="BOTTOM">
	      <input type=HIDDEN name="SEQ" value="{$row.SEQ}">
	      <input type=HIDDEN name="PKG_ID" value="{$row.PKG_ID}">
	      <input type=HIDDEN name="TBL" value="blk_b">
	      <input type=SUBMIT name="EXEC" value="変更">
	      <input type=SUBMIT name="EXEC" value="削除">
	    </td>
	    {/if}
	</tr>
	{/foreach}
	</form>
	<form action="manup.php" method="POST">
	  {for $id=0 to $max_rows-1}
	  <tr>
	    {if $id==0}
	    <td rowspan="{$maxrows2}"><input type=NUMBER class="borderless pkg_id" name="PKG_ID" value="{$next_pkg_id}"></td>
	    {/if}
	    <td><input type=NUMBER class="borderless pkg_id" name="ID" value="{$id}" read_only></td>
	    <td><input type=CHECKBOX class="borderless" name="VALID[{$id}]" value="1"></td>
	    <td>
	      <input type=NUMBER class="borderless num3c" name="STHR[{$id}]" min=0 max=23 step=1 value="0">:
	      <input type=NUMBER class="borderless num3c" name="STMN[{$id}]" min=0 max=59 step=1 value="0">
	    </td>
	    <td>
	      <input type=NUMBER class="borderless num3c" name="EDHR[{$id}]" min=0 max=23 step=1 value="23">:
	      <input type=NUMBER class="borderless num3c" name="EDMN[{$id}]" min=0 max=59 step=1 value="59">
	    </td>
	    <td>
	      <input type=NUMBER class="borderless num3c" name="INMN[{$id}]"  min=0 max=255 step=1 value="0">
	    </td>
	    <td>
	      <input type=NUMBER class="borderless num3c" name="DUMN[{$id}]"  min=0 max=255 step=1 value="0">
	    </td>
	    <td rowspan="2">
	      <input type=NUMBER class="borderless num3c" name="OPE_ID[{$id}]"  min=0 max=255 step=1 value="0">
	    </td>
	    {if $id==0}
	    <td rowspan="{$maxrows2}" valign="BOTTOM">
	      <input type=HIDDEN name="SEQ" value="0">
	      <input type=SUBMIT name="EXEC" value="追加">
	      <input type=HIDDEN name="TBL" value="blk_b">
	    </td>
	    {/if}
	  </tr>
	  <tr>
	    {for $y=0 to 4}
	      {if $y==0}
	    <td colspan="2">
	      {else}
	    <td colspan="1">
	      <select name="CMBOPE[{$id}][{$y}]">
		{$cmbope}
	      </select>
	      {/if}
	      <input list="ccmlist" name="CCMLIST[{$id}][{$y}]" class="num100"/>
	      {$ccmlist}
	      <select name="RLYOPE[{$id}][{$y}]">
		{$rlyope}
	      </select>
	      <input type="NUMBER" name="CMPVAL[{$id}][{$y}]" step=0.01 class="num60"/>
	    </td>
	    {/for}
	  </tr>
	  {/for}
	</form>
      </tbody>
    </table>
  </body>
</html>
