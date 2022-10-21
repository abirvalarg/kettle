#include "strmap.h"
#include <malloc.h>
#include <string.h>

// from https://github.com/rxi/map
static size_t hash(const char *str) {
    size_t hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) ^ *str++;
    }
    return hash;
}

static ktl_StrMapNode *new_node(const char *const key, ktl_Value val)
{
    ktl_StrMapNode *node = malloc(sizeof(ktl_StrMapNode));
    if(node)
    {
        node->hash = hash(key);
        node->next = 0;
        node->key = malloc(strlen(key) + 1);
        strcpy(node->key, key);
        node->value = val;
    }
    return node;
}

void ktl_StrMap_init(ktl_StrMap *m, size_t capacity)
{
    m->buckits = capacity ? malloc(sizeof(ktl_StrMapNode*) * capacity) : 0;
    memset(m->buckits, 0, sizeof(ktl_StrMapNode*) * capacity);
    m->num_buckits = capacity;
    m->num_nodes = 0;
}

void ktl_StrMap_deinit(ktl_StrMap *m)
{
    for(size_t i = 0; i < m->num_buckits; i++)
    {
        ktl_StrMapNode *node = m->buckits[i];
        while(node)
        {
            ktl_StrMapNode *next = node->next;
            free(node->key);
            free(node);
            node = next;
        }
    }
    m->buckits = 0;
    m->num_buckits = 0;
    m->num_nodes = 0;
}

ktl_Value *ktl_StrMap_set(ktl_StrMap *m, const char *const key, ktl_Value value)
{
    if(m->num_nodes == m->num_buckits)
    {
        ktl_StrMapNode *head_node = 0,
            **tail_slot = &head_node;
        for(size_t i = 0; i < m->num_buckits; i++)
        {
            ktl_StrMapNode *cur = m->buckits[i];
            while(cur)
            {
                *tail_slot = cur;
                tail_slot = &cur->next;
                cur = cur->next;
            }
        }

        if(m->buckits)
            free(m->buckits);

        m->num_buckits = m->num_buckits == 0 ? 1 : m->num_buckits * 2;
        m->buckits = malloc(sizeof(ktl_StrMapNode*) * m->num_buckits);
        memset(m->buckits, 0, sizeof(ktl_StrMapNode*) * m->num_buckits);

        while(head_node)
        {
            size_t buckit = head_node->hash % m->num_buckits;
            if(m->buckits[buckit])
            {
                ktl_StrMapNode *cur = m->buckits[buckit];
                while(cur->next)
                    cur = cur->next;
                cur->next = head_node;
            }
            else
                m->buckits[buckit] = head_node;
            ktl_StrMapNode *next = head_node->next;
            head_node->next = 0;
            head_node = next;
        }
    }

    ktl_StrMapNode *node = new_node(key, value);
    if(!node)
        return 0;
    size_t buckit = node->hash % m->num_buckits;
    if(m->buckits[buckit])
    {
        ktl_StrMapNode *cur = m->buckits[buckit];
        char updated = 0;
        if(!strcmp(cur->key, key))
        {
            cur->value = value;
            updated = 1;
            free(node->key);
            free(node);
            node = cur;
        }
        if(!updated)
        {
            while(cur->next)
            {
                cur = cur->next;
                if(!strcmp(cur->key, key))
                {
                    cur->value = value;
                    updated = 1;
                    free(node->key);
                    free(node);
                    node = cur;
                    break;
                }
            }
            if(!updated)
                cur->next = node;
        }
    }
    else
    {
        m->buckits[buckit] = node;
        m->num_nodes++;
    }
    return &node->value;
}

ktl_Value ktl_StrMap_get(ktl_StrMap *m, const char *const key)
{
    size_t buckit = hash(key) % m->num_buckits;
    ktl_StrMapNode *cur = m->buckits[buckit];
    while(cur)
    {
        if(!strcmp(key, cur->key))
            return cur->value;
    }
    return (ktl_Value){
        .type = KTL_NIL,
        .value = { .as_int = 0 }
    };
}

void ktl_StrMap_remove(ktl_StrMap *m, const char *const key)
{
    size_t buckit = hash(key) % m->num_buckits;
    ktl_StrMapNode *cur = m->buckits[buckit],
        **last_slot = &m->buckits[buckit];
    while(cur)
    {
        if(!strcmp(key, cur->key))
        {
            *last_slot = cur->next;
            free(cur->key);
            free(cur);
        }
        last_slot = &cur->next;
        cur = cur->next;
    }
}

ktl_StrMapIter ktl_StrMap_iter(ktl_StrMap *m)
{
    ktl_StrMapNode *cur = 0;
    size_t buckit = 0;
    if(m->num_nodes)
    {
        for(; buckit < m->num_buckits; buckit++)
        {
            if(m->buckits[buckit])
            {
                cur = m->buckits[buckit];
                break;
            }
        }
    }

    return (ktl_StrMapIter){
        .m = m,
        .buckit = buckit,
        .cur = cur
    };
}

ktl_StrMapNode *ktl_StrMap_next(ktl_StrMapIter *iter)
{
    if(iter->cur)
    {
        ktl_StrMapNode *res = iter->cur;
        iter->cur = iter->cur->next;
        if(!iter->cur)
        {
            iter->buckit++;
            for(; iter->buckit < iter->m->num_buckits; iter->buckit++)
            {
                if(iter->m->buckits[iter->buckit])
                {
                    iter->cur = iter->m->buckits[iter->buckit];
                    break;
                }
            }
        }
        return res;
    }
    else
        return 0;
}
