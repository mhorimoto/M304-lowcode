	    <tr>
	      <th rowspan="2">No.</th>
	      <th width="250">値設定</th>
	      <th>桁数</th>
	      <th>単位</th>
	      <th>Lev</th>
	      <th>ROOM/REGION/ORDER/PRIORITY</th>
	    </tr>
	    <tr>
	      <th>CCM名称</th>
	      <th colspan="4"></th>
	    </tr>
	    <tr>
	      <td rowspan="2">0</td>
	      <td>{$TXMETHOD[0].desc}</td>
	      <td><input type="HIDDEN" name="TX[0].cast" value="{$TXMETHOD[0].cast}">{$TXMETHOD[0].cast}</td>
	      <td><input type="HIDDEN" name="TX[0].unit" value="{$TXMETHOD[0].unit}">{$TXMETHOD[0].unit}</td>
	      <td>A-1S-1</td>
	      <input type="HIDDEN" name="TX[0].Lev" value="{$TXMETHOD[0].lv}">
	      <td>
		<input type="NUMBER" name="TX[0].room" class="txt3" min="0" max="127" size="3" maxlength="3" value="{$TXMETHOD[0].room}">/
		<input type="NUMBER" name="TX[0].region" class="txt3" min="0" max="127" size="3" maxlength="3" value="{$TXMETHOD[0].region}">/
		<input type="NUMBER" name="TX[0].order" class="txt5" min="0" max="30000" size="5" maxlength="5" value="{$TXMETHOD[0].order}">/29
		<input type="HIDDEN" name="TX[0].priority" value="{$TXMETHOD[0].pri}">
	      </td>
	    </tr>
	    <tr>
	      <td><input type="TEXT" name="TX[0].ccmtype" size="20" maxlength="20" value="{$TXMETHOD[0].ccm}"></td>
	      <td colspan="4">設定内容</td>
	    </tr>
	    {for $idx=1 to 9}
	    <tr>
	      <td rowspan="2">{$idx}</td>
	      <td>{$TXMETHOD[$idx].desc}</td>
	      <td><input type="NUMBER" name="TX[{$idx}].cast" class="txt3" min="0" max="9" size="1" maxlength="1" value="0"></td>
	      <td><input type="TEXT" name="TX[{$idx}].unit" size="10" maxlength="10" placeholder="単位" value=""></td>
	      <td>
		<select name="TX[{$idx}].Lev">
		  {foreach $LV as $txlev}
		  {html_options values=$txlev.v output=$txlev.name selected=$TXLEVSEL}
		  {/foreach}
		</select>
	      </td>
	      <td>
		<input type="NUMBER" name="TX_ROOM[{$idx}]" class="txt3" min="0" max="127" size="3" maxlength="3" value="{$TXMETHOD[$idx].room}">/
		<input type="NUMBER" name="TX_REGION[{$idx}]" class="txt3" min="0" max="127" size="3" maxlength="3" value="{$TXMETHOD[$idx].region}">/
		<input type="NUMBER" name="TX_ORD[{$idx}]" class="txt5" min="0" max="30000" size="5" maxlength="5" value="{$TXMETHOD[$idx].order}">/
		<input type="NUMBER" name="TX_PRIORITY[{$idx}]" class="txt3" min="0" max="30" size="2" maxlength="2" value="{$TXMETHOD[$idx].pri}">
	    </tr>
	    <tr>
	      <td><input type="TEXT" name="TX_CCMTYPE[{$idx}]" size="20" maxlength="20" value="{$TXMETHOD[$idx].ccm}"></td>
	      <td colspan="3">出力内容</td>
	    </tr>
	  {/for}
