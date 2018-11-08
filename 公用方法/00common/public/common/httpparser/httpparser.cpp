#include "httpparser.h"

namespace mw_http_parser
{
    struct http_parser_settings ms_settings;

    class HttpParserSettings
    {
    public:
        HttpParserSettings();

        static int onMessageBegin(struct http_parser*);
        static int onUrl(struct http_parser*, const char*, size_t);
        static int onStatus(struct http_parser*, const char*, size_t);
        static int onHeaderField(struct http_parser*, const char*, size_t);
        static int onHeaderValue(struct http_parser*, const char*, size_t);
        static int onHeadersComplete(struct http_parser*);
        static int onBody(struct http_parser*, const char*, size_t);
        static int onMessageComplete(struct http_parser*); 
		static int onChunkHeader(struct http_parser*); 
		static int onChunkComplete(struct http_parser*); 
    };

    HttpParserSettings::HttpParserSettings()
    {
        ms_settings.on_message_begin = &HttpParserSettings::onMessageBegin;
        ms_settings.on_url = &HttpParserSettings::onUrl;
        ms_settings.on_status = &HttpParserSettings::onStatus;
        ms_settings.on_header_field = &HttpParserSettings::onHeaderField;
        ms_settings.on_header_value = &HttpParserSettings::onHeaderValue;
        ms_settings.on_headers_complete = &HttpParserSettings::onHeadersComplete;
        ms_settings.on_body = &HttpParserSettings::onBody;
        ms_settings.on_message_complete = &HttpParserSettings::onMessageComplete; 
		ms_settings.on_chunk_header = &HttpParserSettings::onChunkHeader; 
		//ms_settings.on_chunk_complete = &HttpParserSettings::onChunkComplete; 
    }    

    static HttpParserSettings initObj;

    int HttpParserSettings::onMessageBegin(struct http_parser* parser)
    {
        HttpParser* p = (HttpParser*)parser->data;
        return p->onParser(HttpParser::Parser_MessageBegin, 0, 0);
    }

    int HttpParserSettings::onUrl(struct http_parser* parser, const char* at, size_t length)
    {
        HttpParser* p = (HttpParser*)parser->data;
        return p->onParser(HttpParser::Parser_Url, at, length);
    }

    int HttpParserSettings::onStatus(struct http_parser* parser, const char* at, size_t length)
    {
        HttpParser* p = (HttpParser*)parser->data;
        return p->onParser(HttpParser::Parser_Status, at, length);
    }

    int HttpParserSettings::onHeaderField(struct http_parser* parser, const char* at, size_t length)
    {
        HttpParser* p = (HttpParser*)parser->data;
        return p->onParser(HttpParser::Parser_HeaderField, at, length);
    }

    int HttpParserSettings::onHeaderValue(struct http_parser* parser, const char* at, size_t length)
    {
        HttpParser* p = (HttpParser*)parser->data;
        return p->onParser(HttpParser::Parser_HeaderValue, at, length);
    }

    int HttpParserSettings::onHeadersComplete(struct http_parser* parser)
    {
        HttpParser* p = (HttpParser*)parser->data;
        return p->onParser(HttpParser::Parser_HeadersComplete, 0, 0);
    }

    int HttpParserSettings::onBody(struct http_parser* parser, const char* at, size_t length)
    {
        HttpParser* p = (HttpParser*)parser->data;
        return p->onParser(HttpParser::Parser_Body, at, length);
    }

    int HttpParserSettings::onMessageComplete(struct http_parser* parser)
    {
        HttpParser* p = (HttpParser*)parser->data;
        return p->onParser(HttpParser::Parser_MessageComplete, 0, 0);
    }

	int HttpParserSettings::onChunkHeader(struct http_parser* parser)
    {
        HttpParser* p = (HttpParser*)parser->data;
        return p->onParser(HttpParser::Parser_ChunkHeaser, 0, 0);
    }

	int HttpParserSettings::onChunkComplete(struct http_parser* parser)
    {
        HttpParser* p = (HttpParser*)parser->data;
        return p->onParser(HttpParser::Parser_ChunkComplete, 0, 0);
    }

	HttpParser::HttpParser(enum http_parser_type type, bool bIsFixedUrlEncodedBody)
	{
	  m_parser_type = type;
	  
	  http_parser_init(&m_parser, type);

	  m_parser.data = this;

	  m_bIsFixedUrlEncodedBody = bIsFixedUrlEncodedBody;
	}

	HttpParser::HttpParser(enum http_parser_type type)
    {
    	m_parser_type = type;
		
        http_parser_init(&m_parser, type);
		
        m_parser.data = this;
		
		m_bIsFixedUrlEncodedBody = false;

		m_pHeaderChunk = new chunk_list();
    }

	void HttpParser::reset_parser()
	{
        http_parser_init(&m_parser, m_parser_type);
		
        m_parser.data = this;
		
		m_bIsFixedUrlEncodedBody = false;

		init_class_members();
	}

	//��ʼ�����Ա
	void HttpParser::init_class_members()
    {
    	m_pCurField = NULL;
		m_nCurFieldLen = 0;
		
		//m_strField = "";
        m_itemValue.at	= NULL;
		m_itemValue.len = 0;
		m_mapHeaderFieldValue.clear();
		m_urlencode_body_parser.clear();
		m_bChunkPack   = false;
		m_bParseOver   = false;
		m_curr_header_state = header_state_init;
			
        m_nHttpStatusCode = 0;
		
		m_bIsKeepAlive = false;

		m_nReqType = HTTP_REQUEST_UNKNOWN;
		m_pUrl = NULL;
		m_nUrlLen = 0;

		m_pBody = NULL;
		m_nBodyLen = 0;

		m_nContentType = CONTENT_TYPE_UNKNOWN;
		m_pContentType = NULL;												//content-type��ָ��
		m_nContentTypeLen = 0;											//content-type���ݵĳ���
		
		m_pHost = NULL;														//host��ָ��
		m_nHostLen = 0;													//host���ݵĳ���
		
		m_pXForwardedFor = NULL;												//x_forwarded_for��ָ��
		m_nXForwardedForLen = 0;											//x_forwarded_for�ĳ���
		
		m_pExpect = NULL;													//except��ָ��
		m_nExpectLen = 0;												//except�ĳ���

		m_pSoapAction = NULL;												//soapaction��ָ��
		m_nSoapActionLen = 0;											//soapaction�ĳ���
		
		m_pUserAgent = NULL; 												//UserAgent��ָ��
		m_nUserAgentLen = 0; 											//UserAgent�ĳ���
		
		clear_chunk_list();
	}
	
    HttpParser::~HttpParser()
    {
		init_class_members();
		
		delete m_pHeaderChunk;
    }

    int HttpParser::onParser(Event event, const char* at, size_t length)
    {
        switch(event)
        {
            case Parser_MessageBegin:
                return handleMessageBegin();
            case Parser_Url:
                return handleUrl(at, length);
            case Parser_Status:
                return handleStatus(at, length);
            case Parser_HeaderField:
                return handleHeaderField(at, length);
            case Parser_HeaderValue:
                return handleHeaderValue(at, length);
            case Parser_HeadersComplete:
                return handleHeadersComplete();
            case Parser_Body:
                return handleBody(at, length);
            case Parser_MessageComplete:
                return handleMessageComplete();
			case Parser_ChunkHeaser:
                return handleChunkHeader();
			case Parser_ChunkComplete:
                return handleChunkComplete();
            default:
                break;
        }

        return 0;
    }

    int HttpParser::handleMessageBegin()
    {
        init_class_members();
        return onMessageBegin();
    }      
	
	int HttpParser::handleStatus(const char* at, size_t length)
    {	
		m_nHttpStatusCode = atoi(at);
        return 0;
    }
        
	int HttpParser::handleBody(const char* at, size_t length)
    {	
		/*
		if (m_bIsFixedUrlEncodedBody)
		{
			//����body
			m_urlencode_body_parser.parse_http_body(at, length);
		}
		*/
		if (m_bChunkPack)
		{
			m_pCurChunk->block.at	= (char*)at;
			m_pCurChunk->block.len	= length;
			m_pCurChunk->next = new chunk_list();
			m_pCurChunk = m_pCurChunk->next;
			m_nBodyLen += length;
		}
		else
		{
			m_pBody	= (char*)at;
			m_nBodyLen = length;
		}
        return 0;
    }

	int HttpParser::handleUrl(const char* at, size_t length)
    {	
    	if (NULL == m_pUrl)
    	{
			m_pUrl = (char*)at;
    	}
		m_nUrlLen += length;
        return 0;
    }

    int HttpParser::handleHeaderField(const char* at, size_t length)
    {
		//m_strField.append(at, length);

		if (NULL == m_pCurField) m_pCurField = (char*)at;
		m_nCurFieldLen += length;

		size_t nLen = m_nCurFieldLen;
		const char* p = (const char*)m_pCurField;
		
		switch (nLen)
		{
		case 4://host
			{
				if (0 == http_strnicmp(p, "host", nLen))
				{
					m_curr_header_state = header_state_host;
				}
			}
			break;
		case 15://x_forwarded_for
			{
				if (0 == http_strnicmp(p, "x_forwarded_for", nLen))
				{
					m_curr_header_state = header_state_x_forwarded_for;
				}
			}
			break;
		case 12://content-type
			{
				if (0 == http_strnicmp(p, "content-type", nLen))
				{
					m_curr_header_state = header_state_content_type;
				}
			}
			break;
		case 6://expect
			{
				if (0 == http_strnicmp(p, "expect", nLen))
				{
					m_curr_header_state = header_state_expect;
				}
			}
			break;
		case 10://soapaction or User-Agent
			{
				if (0 == http_strnicmp(p, "user-agent", nLen))
				{
					m_curr_header_state = header_state_user_agent;
				}
				else if (0 == http_strnicmp(p, "soapaction", nLen))
				{
					m_curr_header_state = header_state_soap_action;
				}
			}
			break;
		default:
			//���������ݲ�����
			m_curr_header_state = header_state_init;
			break;
		}
        return 0;
    }
        
    int HttpParser::handleHeaderValue(const char* at, size_t length)
    {
		switch (m_curr_header_state)
		{
		case header_state_content_type:
			{
				m_pContentType    = (char*)at;
				m_nContentTypeLen = length;
			}
			break;
		case header_state_x_forwarded_for:
			{
				m_pXForwardedFor	= (char*)at;
				m_nXForwardedForLen = length;
			}
			break;
		case header_state_host:
			{
				m_pHost		= (char*)at;
				m_nHostLen	= length;
			}
			break;
		case header_state_expect:
			{
				m_pExpect		= (char*)at;
				m_nExpectLen	= length;
			}
			break;
		case header_state_soap_action:
			{
				m_pSoapAction		= (char*)at;
				m_nSoapActionLen	= length;
			}
			break;
		case header_state_user_agent:
			{
				m_pUserAgent		= (char*)at;
				m_nUserAgentLen		= length;
			}
			break;
		default:
			break;
		}

		//ָ��ͳ�������
		m_pCurField = NULL;
		m_nCurFieldLen = 0;
		//ͷ״̬Ϊ��ʼ
		m_curr_header_state = header_state_init;
		/*
        m_itemValue.at  = (char*)at;
		m_itemValue.len = length;
		to_upper_s(m_strField);
		m_mapHeaderFieldValue.insert(std::make_pair(m_strField, m_itemValue));
		m_strField.clear();
		m_curr_header_state = header_state_init;
		*/
        return 0;
    }
        
    int HttpParser::handleHeadersComplete()
    {
		m_bIsKeepAlive = http_should_keep_alive(&m_parser);
		m_nContentType = parse_content_type();
		m_nReqType     = parse_request_type();
		
        return onHeadersComplete();
    }
	char* strnchr(const char* s, char c, int pos)
    {
    	char* p = (char*)s;
    	while (*s != '\0' && pos > 0)
    	{
    		if (*p == c) 
			{
				return p;
    		}
    		--pos;
			++p;
    	}
		return NULL;
    }
	int HttpParser::handleMessageComplete()
    {		
		//����Ҫ�����Ƿ���GET����ȡ��GET�����URL��BODY
		if (HTTP_REQUEST_GET == m_nReqType)
		{
			char* pHelp = strnchr(m_pUrl, '?', 128);
			if (pHelp)
			{
				m_pBody = pHelp+1;
				int urllen = pHelp-m_pUrl;
				if (urllen >= 0)
				{
					int bodylen = m_nUrlLen-urllen-1;
					m_nBodyLen  = bodylen<0?0:bodylen;
					m_nUrlLen   = urllen;
				}
				else
				{
					m_nUrlLen  = 0;
					m_nBodyLen = 0;
				}
			}
		}

		m_bParseOver = true;
		
        return onMessageComplete();
    }

	int HttpParser::handleChunkHeader()
    {		
		m_bChunkPack = true;
        return onChunkHeader();
    }

	int HttpParser::handleChunkComplete()
    {		
        return onChunkComplete();
    }

//..........................�����ṩһ�����urlencode�ĺ���......................
	//ͨ��key��ȡvalue��ָ��ͳ���,�������忽��.������false����key������
	//ע��:�����ȵ���parse_http_body��ſ���ʹ�øú���
	bool HttpParser::get_value(const char* key/*key,��:cmd=aa,cmd����key,aa����value*/, char** value, size_t* len)
	{
		return m_urlencode_body_parser.get_value(key, value, len);
	}

	//ͨ��key��ȡvalue�����ݺͳ���.������false����key������
	//ע��:�����ȵ���parse_http_body��ſ���ʹ�øú���
	bool HttpParser::get_value(const char* key/*key,��:cmd=aa,cmd����key,aa����value*/, std::string& value)
	{
		return m_urlencode_body_parser.get_value(key, value);
	}

	//����key,value��ʽ��body,��:cmd=aa&seqid=1&msg=11
	//ע��:����GetContentType����CONTENT_TYPE_URLENCODE�ſ���ʹ�øú���ȥ����
	bool HttpParser::parse_http_body(const char* p/*http body ��:cmd=aa&seqid=1&msg=11*/, size_t len/*http body�ĳ���*/)
	{
		return m_urlencode_body_parser.parse_http_body(p, len);
	}
//................................................................................

    //ִ�н���.
	//����ִ��N��,���һ��http����,����һ���յ���,���Զ�ε���
	//����ֵ:0:���ν���OK  ��0:���ν���ʧ�� ��ʧ�������  ��ȡʧ��ԭ��
	//��over����true,������ɵ�http���Ľ��������,���Ե�������������http���ݻ�ȡ
	int HttpParser::execute_http_parse(const char* data/*����Ҫ����������*/, size_t len/*����Ҫ���������ݳ���*/, bool& over/*�����Ƿ������http���������*/)
    {
		int nErrCode = 0;

        size_t parsed = http_parser_execute(&m_parser, &ms_settings, data, len);
        if (m_parser.upgrade)
        {
            onUpgrade(data + parsed, len - parsed); 
        }
        else if (parsed != len)
        {
			nErrCode = m_parser.http_errno;
        }     
		else
		{
			nErrCode = 0;
		}
		
		over = m_bParseOver;

        return nErrCode;
    }

	//����execute_http_parse���ص�ʧ�ܴ���,��ȡʧ��ԭ��
	const char* HttpParser::get_http_parse_errno_description(int errno)
	{
		return http_errno_description((enum http_errno)errno);
	}

	//��ȡhttp��Ӧ��״̬��,��200,202,400,403,404,500��
	//ֻ�е���ʼ��ΪHTTP_RESPONSE����ʱ���ܻ�ȡ����ȷ��ֵ
	//����ֵ:״̬��
	int HttpParser::get_http_response_status_code()
    {
		return m_nHttpStatusCode;
    }

	//�Ƿ���Ҫ��������,��connection��keep-alive����close
	//true:keep-alive false:close
	bool HttpParser::should_keep_alive()
	{
		return m_bIsKeepAlive;
	}

	//�Ƿ���100 continue
	bool HttpParser::expect_100_continue()
	{
		bool bRet = false;
		if (m_pExpect != NULL)
		{
			//100-continue
			if (12 == m_nExpectLen && 0 == http_strnicmp(m_pExpect, "100-continue", m_nExpectLen))
			{
				bRet = true;
			}
		}
		return bRet;
	}

	//�Ƿ���chunk��
	//����ֵ:true:chunk�� false:��chunk��
	bool HttpParser::is_chunk_body()
	{
		return m_bChunkPack;
	}

	/*
	//��ȡhttpͷ�е��� �� ֵ
	//����falseʱ��ʾͷ��û�и���
	bool HttpParser::get_http_header_filed_value(const char* field, char** value, size_t* len)
	{
		bool bRet = false;
		std::string strField = field;
		to_upper_s(strField);
		std::map<std::string, item_value>::iterator it = m_mapHeaderFieldValue.find(strField);
		if (it != m_mapHeaderFieldValue.end())
		{
			*value = it->second.at;
			*len   = it->second.len;
			bRet   = true;
		}

		return bRet;
	}
	
	//��ȡhttpͷ�е��� �� ֵ  !!!����ʹ��bIsFixedUrlEncodedBodyΪtrueʱ���ܻ�ȡ����Ҫ������
	//����falseʱ��ʾͷ��û�и���
	bool HttpParser::get_http_body_filed_value(const char* field, char** value, size_t* len)
	{
		bool bRet = false;
		if (m_bIsFixedUrlEncodedBody)
		{
			bRet = m_urlencode_body_parser.get_value(field, value, len);
		}
		return bRet;
	}
	*/

	//��ȡhttp body�ĳ���
	//����ֵ:body�ĳ���
	size_t HttpParser::get_http_body_len()
	{
		return m_nBodyLen;
	}

	//��ȡhttp body������
	//����ֵ:body��ָ��ͳ���
	size_t HttpParser::get_http_body_data(char** data, size_t* len)
	{
		*data = m_pBody;
		*len  = m_nBodyLen;
		return m_nBodyLen;
	}

	//��ȡchunk����http body������
	//����ֵ:body�ĳ���
	//ע��:�����is_chunk_body���ж��Ƿ���chunk��,Ȼ�����get_http_body_len�õ����ȷ����㹻�Ĵ�С��data
	size_t HttpParser::get_http_body_chunk_data(char* data)
	{
		chunk_list* next = m_pHeaderChunk->next;
		size_t len = 0;

		if (m_pHeaderChunk->block.at != NULL)
		{
			memcpy(data + len, m_pHeaderChunk->block.at, m_pHeaderChunk->block.len);
			len += m_pHeaderChunk->block.len;
		}

		for (;;)
		{
			if (next != NULL)
			{
				chunk_list* p = next->next;
				
				if (next->block.at != NULL)
				{
					memcpy(data + len, next->block.at, next->block.len);
					len += next->block.len;
				}

				next = p;
			}
			else
			{
				break;
			}
		}
		return len;
	}

	//��ȡhost
	//����ֵ:host��ָ��,�Լ�����
	size_t HttpParser::get_http_host(char** host, size_t* len)
	{
		size_t retlen = 0;
		if (m_pHost != NULL)
		{
			*host = m_pHost;
			*len  = m_nHostLen;
			retlen= *len;
		}
		return retlen;
		/*
		size_t retlen = 0;

		std::map<std::string, item_value>::iterator it = m_mapHeaderFieldValue.find("HOST");
		if (it != m_mapHeaderFieldValue.end())
		{
			*host	= it->second.at;
			*len	= it->second.len;
			retlen	= *len;	
		}
		
		return retlen;
		*/
	}

	//��ȡ����url
	//����ֵ:url��ָ��,�Լ�����
	size_t HttpParser::get_http_url(char** url, size_t* len)
	{
		size_t retlen = 0;
		if (m_pUrl != NULL)
		{
			*url  = m_pUrl;
			*len  = m_nUrlLen;
			retlen = *len;
		}
		return retlen;
	}

	//��ȡUser-Agent
	//����ֵ:User-Agent��ָ��,�Լ�����
	size_t	HttpParser::get_http_user_agent(char** useragent, size_t* len)
	{
		size_t retlen = 0;
		if (m_pUserAgent != NULL)
		{
			*useragent  = m_pUserAgent;
			*len  		= m_nUserAgentLen;
			retlen = *len;
		}
		return retlen;
	}

	//��ȡcontent-type
	//����ֵ:CONTENT_TYPE_XML ���� xml,CONTENT_TYPE_JSON ���� json,CONTENT_TYPE_URLENCODE ���� urlencode,CONTENT_TYPE_UNKNOWN ���� δ֪����
	//˵��:��ֻ�ṩ������
	int HttpParser::get_http_content_type()
	{
		return m_nContentType;
	}
	int HttpParser::parse_content_type()
	{
		int contenttype = CONTENT_TYPE_UNKNOWN;
		if (m_pContentType != NULL)
		{
			size_t len = 0;
			char* p = m_pContentType;
			while (len < m_nContentTypeLen)
			{
				if (p[len] == '/')
				{
					if (m_nContentTypeLen - len >= 3 && 0 == http_strnicmp(p+len+1, "xml", 3))
					{
						contenttype = CONTENT_TYPE_XML;
						break;
					}
					else if (m_nContentTypeLen - len >= 4 && 0 == http_strnicmp(p+len+1, "json", 4))
					{
						contenttype = CONTENT_TYPE_JSON;
						break;
					}
					else if (m_nContentTypeLen - len >= 21 && 0 == http_strnicmp(p+len+1, "x-www-form-urlencoded", 21))
					{
						contenttype = CONTENT_TYPE_URLENCODE;
						break;
					}
					else
					{
						++len;
					}
				}
				else
				{
					++len;
				}
			}
		}
		return contenttype;
		/*
		size_t retlen = 0;
		
		std::map<std::string, item_value>::iterator it = m_mapHeaderFieldValue.find("CONTENT-TYPE");
		if (it != m_mapHeaderFieldValue.end())
		{
			*content_type	= it->second.at;
			*len			= it->second.len;
			retlen			= *len;	
		}
		
		return retlen;
		*/
	}

	//��ȡX-Forwarded-For
	//����ֵ:X-Forwarded-For��ָ��,�Լ�����
	size_t HttpParser::get_http_x_forwarded_for(char** x_forwarded_for, size_t* len)
	{
		size_t retlen = 0;
		if (m_pXForwardedFor != NULL)
		{
			*x_forwarded_for	= m_pXForwardedFor;
			*len				= m_nXForwardedForLen;
			retlen = *len;
		}
		return retlen;
		/*
		size_t retlen = 0;
		
		std::map<std::string, item_value>::iterator it = m_mapHeaderFieldValue.find("X-FORWARDED-FOR");
		if (it != m_mapHeaderFieldValue.end())
		{
			*x_forwarded_for	= it->second.at;
			*len				= it->second.len;
			retlen				= *len;	
		}
		
		return retlen;
		*/
	}

	//��ȡSOAPAction
	//����ֵ:SOAPAction��ָ��,�Լ�����
	size_t HttpParser::get_http_soapaction(char** soapaction, size_t* len)
	{
		size_t retlen = 0;
		if (m_pSoapAction != NULL)
		{
			*soapaction	= m_pSoapAction;
			*len		= m_nSoapActionLen;
			retlen = *len;
		}
		return retlen;
	}

	//��ȡhttp���������
	//����ֵ:HTTP_REQUEST_POST ���� post����; HTTP_REQUEST_GET ���� get���� ; HTTP_REQUEST_UNKNOWN ���� δ֪(������ͷ�д���)
	int HttpParser::get_http_request_type()
	{
		return m_nReqType;
	}
	int HttpParser::parse_request_type()
	{
		char* p = m_pUrl;
		if (p != NULL 
			&& ((*(p-2) == 'T' || *(p-2) == 't')) 
			&& ((*(p-3) == 'S' || *(p-3) == 's'))
			&& ((*(p-4) == 'O' || *(p-4) == 'o'))
			&& ((*(p-5) == 'P' || *(p-5) == 'p')))
		{
			return HTTP_REQUEST_POST;	
		}
		else if (p != NULL 
			&& ((*(p-2) == 'T' || *(p-2) == 't')) 
			&& ((*(p-3) == 'E' || *(p-3) == 'e'))
			&& ((*(p-4) == 'G' || *(p-4) == 'g')))
		{
			return HTTP_REQUEST_GET;
		}
		else
		{
			return HTTP_REQUEST_UNKNOWN;
		}
	}
}

