<% do_hpagehead("wol.titl"); %> 
   <body> 
      <div id="header"> 
         <div class="logo"> </div> 
         <div class="navig"><a href="index.asp">Index</a> | <a href="javascript:self.close();"><% tran("sbutton.clos"); %></a></div> 
      </div> 
      <div id="content"> 
         <h2><% tran("wol.h2"); %></h2> 
         <dl> 
            <dd>Wake-on-LAN�ϡ����ε����LAN¦�ͥåȥ����Υۥ��Ȥ����Ϥ��鵯ư���뤿��ε�ǽ�Ǥ���</dd> 
               <dt class="term">���Хۥ���</dt> 
               <dd class="definition">LAN¦�ˤ���ۥ��Ȥ�ɽ�����ޤ���������List��DHCP�����Ф˴�������Ƥ����Ρ���ư���ɲä��줿��Τ�ޤߤޤ���
               <div class="note"> 
                  <h4>���</h4> 
                     <div>������MAC���ɥ쥹�ϡ�LAN¦�ͥåȥ����Ʊ��ͥåȥ�����ɥ쥹���ޥ����˴ޤޤ�뤳�Ȥ��ꤷ�Ƥ��ޤ����ޤ�����ư�����Ѥ���Τ�UDP�ѥ��åȤǤ�(���ꤷ�ʤ��ä���硢�ݡ����ֹ��7����Ѥ��ޤ�)</div> 
               </div> 
            </dd> 
               <dt class="term"><% tran("wol.legend2"); %></dt> 
               <dd class="definition">�ºݤ�WOL��¹Ԥ��뤿�����Ͽ���줿�ꥹ�ȤǤ�(���Υꥹ�Ȥ��Ÿ����ڤäƤ⼺���ޤ���)�����Υꥹ�Ȥϼ�ưŪ����Ͽ���줿��Τȼ�ư��Ͽ���줿��Τ�ξ����ɽ������ޤ���
               <dt class="term"><% tran("wol.legend4"); %></dt> 
               <dd class="definition">
				�ֵ�ư�ץܥ���򥯥�å����뤳�Ȥˤ�äơ����꡼����Υꥹ����Υۥ��Ȥ�ư�����뤳�Ȥ��Ǥ��ޤ���
               <dt class="term"><% tran("wol.mac"); %></dt> 
               <dd class="definition">MAC���ɥ쥹�����(���ɥ쥹�֤ϥ��ڡ����Ƕ��ڤ�ɬ�פ�����ޤ�)���뤳�Ȥˤ�äơ������ε������٤˵�ư�����뤳�Ȥ��Ǥ��ޤ���

               <div class="note"> 
                  <h4>���</h4> 
                     <div>�������MAC���ɥ쥹�⼡�Υե����ޥåȤ����Ϥ���ɬ�פ�����ޤ���xx:xx:xx:xx:xx:xx��</div> 
                  </div> 
               </dd> 
               <dt class="term"><% tran("wol.broadcast"); %></dt> 
                      <dd class="definition">MAC���ɥ쥹���б�����֥��ɥ��㥹��IP�����ꤷ�ޤ�</dd> 
               <dt class="term"><% tran("wol.udp"); %></dt> 
               <dd class="definition">��������֥��ɥ��㥹�ȥѥ��åȤ�UDP�ݡ��Ȥ���ꤷ�ޤ���</dd> 
               <dt class="term"><% tran("sbutton.wol"); %></dt> 
               <dd class="definition">��ư�ǵ�ư��Ԥ��ޤ�����ư���˻��Ѥ����ѥ�᡼����<b>manual_wol_mac</b>, <b>manual_wol_network</b>, <b>manual_wol_port</b>�γ�NVRAM�ѥ�᡼������¸����ޤ���</dd> 
         </dl> 
      </div> 
   </body> 
</html> 

