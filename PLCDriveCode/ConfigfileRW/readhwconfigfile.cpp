#include "readhwconfigfile.h"


ReadHWconfigFile::ReadHWconfigFile()
{
}
QString ReadHWconfigFile::ReadHWconfigType(HWCONFIG_STRU &stru, QString filename)
{
    QString msg("");

     QFile file(filename);
     if(!file.exists())//文件不存在
     {
         msg = QString("%1 is not exists!").arg(filename);
     }

     if(file.open(QIODevice::ReadOnly | QIODevice::Text))
     {
         //构建QXmlStreamReader对象
         QXmlStreamReader reader(&file);
         while(!reader.atEnd())//没有读到XML结尾
         {
             reader.readNext();//读下一个节点
             //判断是否是节点的开始
             if(reader.isStartElement())
             {
                 //判断当前节点的名字
                 if(reader.name() == "netName")
                 {
                   stru.netName = reader.readElementText();
                 }
                 if(reader.name() == "INPUT")
                 {
                     ReadHWInputType(reader,"INPUT",stru.hwconfigInputMap);
                 }
                 if(reader.name() == "OUTPUT")
                 {
                     ReadHWOutputType(reader,"OUTPUT",stru.hwconfigOutputMap);
                 }
             }
             //节点结束、并且节点名字为Data（含有子节点）
             else if(reader.isEndElement() && reader.name() == "Data")//读到xml结尾的data节点
             {
                 break;
             }
         }
         file.close();
     }
     else
     {
         msg = QString("Open file %1 failure").arg(filename);//打开失败
     }

     return msg;
}

void ReadHWconfigFile::ReadHWInputType(QXmlStreamReader &reader, QString strElement, QMap<int, QMap<int, HWCONFIGPROPERTY_STRU> > &hwconfigInputMap)
{
    //INPUT读的方式
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name()=="类型")
            {
                QXmlStreamAttributes attributes = reader.attributes();
                //判断是否存在属性
                if(attributes.hasAttribute("type"))
                {
                    QMap<int, HWCONFIGPROPERTY_STRU>  inputtypeMap ;
                    QString typeID = attributes.value("type").toString();
                    while(!reader.atEnd())
                    {
                        reader.readNext();
                        if(reader.isStartElement())
                        {
                            if(reader.name()=="property")
                            {
                                HWCONFIGPROPERTY_STRU propertystru;//先定义一个存放节点属性的结构体
                                QXmlStreamAttributes attributes = reader.attributes();
                                if(attributes.hasAttribute("addr"))
                                {
                                    propertystru.addr = attributes.value("addr").toInt();
                                }
                                if(attributes.hasAttribute("size"))
                                {
                                    propertystru.size=attributes.value("size").toInt();
                                }
                                if(attributes.hasAttribute("haType"))
                                {
                                    propertystru.hwtype=attributes.value("haType").toString();
                                }
                                if(inputtypeMap.contains(propertystru.addr))
                                {
                                    inputtypeMap[propertystru.addr] = propertystru;
                                }
                                else{
                                    inputtypeMap.insert(propertystru.addr,propertystru);
                                }
                            }
                        }
                        if(reader.isEndElement())
                        {
                            if(reader.name() == "类型")
                            {
                                break;
                            }
                        }
                    }
                    int type = -1;
                    if(typeID == "ec")
                    {
                        type = 0;
                    }
                    else if(typeID == "can")
                    {
                       type = 1 ;
                    }
                    if(!hwconfigInputMap.contains(type))
                    {
                        hwconfigInputMap.insert(type,inputtypeMap);
                    }
                    else{
                        hwconfigInputMap[type] = inputtypeMap;
                    }

                }
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}

void ReadHWconfigFile::ReadHWOutputType(QXmlStreamReader &reader, QString strElement, QMap<int, QMap<int, HWCONFIGPROPERTY_STRU> > &hwconfigOutputMap)
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if(reader.name()=="类型")
            {
                QXmlStreamAttributes attributes = reader.attributes();
                //判断是否存在属性
                if(attributes.hasAttribute("type"))
                {
                    QMap<int, HWCONFIGPROPERTY_STRU>  outputtypeMap ;
                    QString typeID = attributes.value("type").toString();
                    while(!reader.atEnd())
                    {
                        reader.readNext();
                        if(reader.isStartElement())
                        {
                            if(reader.name()=="property")
                            {
                                HWCONFIGPROPERTY_STRU propertystru;//先定义一个存放节点属性的结构体
                                QXmlStreamAttributes attributes = reader.attributes();
                                if(attributes.hasAttribute("addr"))
                                {
                                    propertystru.addr = attributes.value("addr").toInt();
                                }
                                if(attributes.hasAttribute("size"))
                                {
                                    propertystru.size = attributes.value("size").toInt();
                                }
                                if(attributes.hasAttribute("haType"))
                                {
                                    propertystru.hwtype=attributes.value("haType").toString();
                                }
                                if(outputtypeMap.contains(propertystru.addr))
                                {
                                    outputtypeMap[propertystru.addr] = propertystru;
                                }
                                else{
                                    outputtypeMap.insert(propertystru.addr,propertystru);
                                }
                            }
                        }
                        if(reader.isEndElement())
                        {
                            if(reader.name() == "类型")
                            {
                                break;
                            }
                        }

                    }
                    int type = -1;
                    if(typeID == "ec")
                    {
                        type = 0;
                    }
                    else if(typeID == "can")
                    {
                       type = 1 ;
                    }
                    if(!hwconfigOutputMap.contains(type))
                    {
                        hwconfigOutputMap.insert(type,outputtypeMap);
                    }
                    else{
                        hwconfigOutputMap[type] = outputtypeMap;
                    }
                }
            }
        }
        if(reader.isEndElement())
        {
            if(reader.name() == strElement)
            {
                break;
            }
        }
    }
}
