
#include "linked_list_data.h"


/**
 * ��ǰд�����ݵĽڵ�д��һ���ַ�
 * ����ڵ�д�������ַ��ǽ�����ǣ���ôд�����ݵĽڵ����ӵ���������
 * �������д��ɹ����� 1�����򷵻� 0�����������д���߶���д�����ʧ������0
 */
unsigned int Linked_List_Data_Add_Char0(struct Linked_List_Data *Data_Buff,unsigned char c)
{
	
	unsigned int i;
	unsigned short int new_node;
	
	if(Data_Buff->Current_Node == NULL)
	{
		//������е������нڵ㣬��ôȡ�������ӵ��Ͻڵ㣬����ȡ����������Ͻڵ�
		if(Data_Buff->Free_List == NULL)
		{
			if(Data_Buff->Data_List != NULL)
			{
				Data_Buff->Current_Node = Data_Buff->Data_List;
				Linked_Remove_First( &Data_Buff->Data_List,&Data_Buff->Var_Node );
			}
		}
		else
		{
			Data_Buff->Current_Node = Data_Buff->Free_List;
			Linked_Remove_First( &Data_Buff->Free_List,&Data_Buff->Var_Node );
		}
		
		//û�п��õĽڵ�
		if(Data_Buff->Current_Node == NULL)
		{
			return 0;
		}
		else
		{
			//��������
			Data_Buff->Current_Node_Write_Idx = 0;
			
			for(i = 0;i<Data_Buff->Char_Len;i++)
			{
				Data_Buff->Current_Node->C[i] = 0x00;
			}
		}
	}
	
	//д������
	if(Data_Buff->Current_Node_Write_Idx < Data_Buff->Char_Len)
	{
		Data_Buff->Current_Node->C[ Data_Buff->Current_Node_Write_Idx ] = c;
		Data_Buff->Current_Node_Write_Idx ++;
	}
	
	new_node = 0;

	//д���������Ƿ��н������
	if(Data_Buff->Current_Node_Write_Idx == Data_Buff->Char_Len)
	{
		new_node = 1;
	}
	else
	{	
		//�н�����ǲ���д���㹻���ַ�
		if(Data_Buff->Eof != NULL && Data_Buff->Eof_Len != 0 &&
			Data_Buff->Current_Node_Write_Idx >= Data_Buff->Eof_Len)
		{
			//������Ϊ���Ե���һ���ڵ�
			new_node = 1;
			for(i = 0;i < Data_Buff->Eof_Len;i++)
			{
				//ֻ���ַ���ƥ��
				if(Data_Buff->Current_Node->C[ Data_Buff->Current_Node_Write_Idx - i - 1 ] != Data_Buff->Eof[ Data_Buff->Eof_Len - i - 1 ] )
				{
					new_node = 0;
				}
			}
		}
		
	}
	
	//��ӵ�����	
	if(new_node == 1)
	{
		//�ѵ�ǰ�ڵ�ӵ���������
		Linked_Add_Last( &Data_Buff->Data_List, &Data_Buff->Current_Node,&Data_Buff->Var_Node );

		//�ͷŵ�ǰд��Ľڵ�
		Data_Buff->Current_Node = NULL;
	}

	return 1;

}


unsigned int Linked_List_Data_Add_Char(struct Linked_List_Data *Data_Buff,unsigned char c)
{
	int i;

	if(Data_Buff == NULL)
	{
		return 0;
	}
	
		//����
	if(Data_Buff->Write_Lock | Data_Buff->Read_Lock)
	{
		
		//��BUFF�пռ䣬��ô�浽��BUFF������������
		if(Data_Buff->Lock_Buff_Len < LOCK_BUFFER_LENGTH)
		{
			Data_Buff->Lock_Buff[Data_Buff->Lock_Buff_Len] = c;//��0��ʼ
			Data_Buff->Lock_Buff_Len ++;//���һ�����1
			return 1;
		}
		else
		{
			return 0;
		}
	}
	
	//����
	Data_Buff->Write_Lock = 1;
	
	//�ȴ��������������
	if(Data_Buff->Lock_Buff_Len > 0) //�л��������1��ʼ��Ϊ0��ʾû��
	{
		for(i = 0; i< Data_Buff->Lock_Buff_Len;i++)
		{
			Linked_List_Data_Add_Char0(Data_Buff,Data_Buff->Lock_Buff[i]);
		}
		Data_Buff->Lock_Buff_Len = 0; //����Ϊ0
	}
	
	Linked_List_Data_Add_Char0(Data_Buff,c);
		
	//����
	Data_Buff->Write_Lock = 0;
	
	return 1;
}



/**
 * �����������ж���һ���ڵ�����ݣ�
 * ��������ݷ��� 1�����򷵻� 0�����������д���߶���Ҳ����0
 * ����Data_Buff��ȡ��ᱻ���ǣ�������ݻ�memcpy�� buff
 *
 * 
 */
unsigned int Linked_List_Data_Read(struct Linked_List_Data *Data_Buff,unsigned char * Buff)
{

	
	if(Data_Buff == NULL || Data_Buff->Data_List == NULL)
	{
		return 0;
	}
	
	//����
	if(Data_Buff->Write_Lock | Data_Buff->Read_Lock)
	{
		return 0;
	}
	//����
	Data_Buff->Read_Lock = 1;
		
	//�����ݸ��Ƴ���
	memcpy(Buff,Data_Buff->Data_List->C,Data_Buff->Char_Len);
	
	
	//��¼�Ѷ��ڵ��ָ��
	//Data_Buff->var_node = Data_Buff->data_list;
	//�����ݽڵ�ӵ���������
	Linked_Add_Last( &Data_Buff->Free_List, &Data_Buff->Data_List,&Data_Buff->Var_Node );
	
	//�����ݽڵ�ɾ����һ��(�Ѷ��ڵ�)
	Linked_Remove_First( &Data_Buff->Data_List,&Data_Buff->Var_Node );
	
	//����
	Data_Buff->Read_Lock = 0;
	
	return 1;

}
