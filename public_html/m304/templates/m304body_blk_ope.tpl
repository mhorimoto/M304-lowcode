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
