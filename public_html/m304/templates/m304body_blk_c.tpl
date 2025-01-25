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
