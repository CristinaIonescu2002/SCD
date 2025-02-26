struct requestTokenAcces {
    string user_id<15>;
    string token<15>;
    int type;
};

struct returnRequestTokenAcces {
    string tokenAcces<15>;
    string tokenRegen<15>;
    int period;
};

struct validateDelegatedAction {
    string tipOperatie<>;
    string resursa<>;
    string token<15>;
};

enum AuthResult {
    PERMISSION_GRANTED,
    PERMISSION_DENIED,
    TOKEN_EXPIRED,
    USER_NOT_FOUND,
    REQUEST_DENIED,
    RESOURCE_NOT_FOUND,
    OPERATION_NOT_PERMITTED
};

struct gatAndHandelTtl {
    string token<15>;
    int ttl;
};

program OAUTH {
    version OAUTH_V1 {
        string REQUEST_AUTHORIZATION(string user_id) = 1;
        returnRequestTokenAcces REQUEST_AUTHORIZATION_TOKEN(requestTokenAcces rat) = 2;
        enum AuthResult VALIDATE_DELEGATED_ACTION(validateDelegatedAction vda) = 3;
        string APPROVE_REQUEST_TOKEN(string token) = 4;
        gatAndHandelTtl GET_ACCES_TOKEN_AND_HANDEL_TTL(string user_id) = 5;
        int INCREASE_COUNT(void) = 6;
    } = 1;
} = 0x20000001;
