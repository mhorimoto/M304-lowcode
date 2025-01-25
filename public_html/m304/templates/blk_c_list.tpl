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
    <h1>blk_c編集</h1>
    <p>{$msg}</p>
    {include file='link_list.inc'}
    <table>
      <thead>
	<tr>
	  <th>PKG_ID</t><th>ID</th><th>有効</th><th>ROOM/REGION/ORDER</th><th>寿命</th><th>CCMTYPE</th><th>名称</th><th>操作</th>
	</tr>
      </thead>
      <tbody>
	<form action="manup.php" method="POST">
	{foreach from=$data item=row}
	<tr>
	    {if $row.ID==0}
	    <td rowspan="10"><input type=NUMBER class="borderless pkg_id" name="PKGID" value="{$row.PKG_ID}"></td>
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
	      <input type=NUMBER class="borderless num3c" name="ROOM[{$row.ID}]"  min=0 max=127 step=1 value="{$row.ROOM}">/
	      <input type=NUMBER class="borderless num3c" name="REGION[{$row.ID}]" min=0 max=127 step=1 value="{$row.REGION}">/
	      <input type=NUMBER class="borderless num5c" name="ORD[{$row.ID}]" min=0 max=30000 value="{$row.ORD}">
	    </td>
	    <td>
	      <select name="LIFECOUNT[{$row.ID}]">
		{html_options options=$lifecount_sel selected="{$row.LIFECOUNT}"}
	      </select>
	    </td>
	    <td><input type=TEXT   class="borderless" maxlength=22 name="CCMTYPE[{$row.ID}]" value="{$row.CCMTYPE}"></td>
	    <td><input type=TEXT   class="borderless num200" name="NAME[{$row.ID}]" value="{$row.NAME}"></td>
	    {if $row.ID==0}
	    <td rowspan="10" valign="BOTTOM">
	      <input type=HIDDEN name="SEQ" value="{$row.SEQ}">
	      <input type=HIDDEN name="PKG_ID" value="{$row.PKG_ID}">
	      <input type=HIDDEN name="TBL" value="blk_c">
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
	    <td rowspan="{$max_rows}"><input type=NUMBER class="borderless pkg_id" name="PKG_ID" value="{$next_pkg_id}"></td>
	    {/if}
	    <td><input type=NUMBER class="borderless pkg_id" name="ID" value="{$id}" read_only></td>
	    <td><input type=CHECKBOX class="borderless" name="VALID[{$id}]" value="1"></td>
	    <td>
	      <input type=NUMBER class="borderless num3c" name="ROOM[{$id}]"  min=0 max=127 step=1 value="{$row.ROOM}">/
	      <input type=NUMBER class="borderless num3c" name="REGION[{$id}]" min=0 max=127 step=1 value="{$row.REGION}">/
	      <input type=NUMBER class="borderless num5c" name="ORD[{$id}]" min=0 max=30000 value="{$row.ORD}">
	    </td>
	    <td>
	      <select name="LIFECOUNT[{$id}]">
		{html_options options=$lifecount_sel}
	      </select>
	    </td>
	    <td><input type=TEXT   class="borderless" maxlength=22 name="CCMTYPE[{$id}]" value="{$row.CCMTYPE}"></td>
	    <td><input type=TEXT   class="borderless num200" name="NAME[{$id}]" value="{$row.NAME}"></td>
	    {if $id==0}
	    <td rowspan="{$max_rows}" valign="BOTTOM">
	      <input type=HIDDEN name="SEQ" value="0">
	      <input type=SUBMIT name="EXEC" value="追加">
	      <input type=HIDDEN name="TBL" value="blk_c">
	    </td>
	    {/if}
	  </tr>
	  {/for}
	</form>
      </tbody>
    </table>
  </body>
</html>
