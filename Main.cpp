#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

using Path = std::string;

class Sprite
{
public:
    Sprite(unique_ptr<sf::Texture> texture)
        : texture(*texture)
        , sprite(this->texture)
    {}

    virtual void draw(sf::RenderTarget& at)
    { at.draw(sprite); }

    sf::Texture& getTexture()
    { return texture; }

    sf::Sprite& getSprite()
    { return sprite; }

    const sf::Texture& getTexture() const
    { return texture; }

    const sf::Sprite& getSprite() const
    { return sprite; }

protected:
    sf::Texture texture;
    sf::Sprite sprite;
};

class EnchantedSprite: public Sprite
{
public:
    EnchantedSprite(unique_ptr<sf::Texture> texture, const Path& shaderPath)
        : Sprite(std::move(texture))
    { shader.loadFromFile(shaderPath + ".frag", sf::Shader::Fragment); }

    void draw(sf::RenderTarget& at) override
    { at.draw(sprite, &shader); }

    template<class... Args>
    void setUniform(Args... args)
    { shader.setUniform(args...); }

protected:
    sf::Shader shader;
};

unique_ptr<sf::Texture> loadTexture(const Path& path)
{
    auto res = make_unique<sf::Texture>();
    res->loadFromFile("Sources/" + path + ".png");
    return res;
}

const float pi = acos(-1);
const int imageScale = 8;

int main()
{
    const int windowSize = 60;

    sf::RenderWindow window(sf::VideoMode(windowSize * imageScale, windowSize * imageScale), "Enchanter", sf::Style::Close);
    window.setFramerateLimit(60);
    window.setView(sf::View({ 0, 0, (float)windowSize, (float)windowSize }));

    vector<Sprite*> items {
        // new EnchantedSprite(loadTexture("Void"), "MainShader"),
        new EnchantedSprite(loadTexture("NetheriteHelmet"), "MainShader"),
        new EnchantedSprite(loadTexture("NetheriteChestplate"), "MainShader"),
        new EnchantedSprite(loadTexture("NetheriteLeggings"), "MainShader"),
        new EnchantedSprite(loadTexture("NetheriteBoots"), "MainShader"),
    };

    const float maxSize = (*max_element(items.begin(),
                                        items.end(),
                                        [](const Sprite* a, const Sprite* b)
                                        { return a->getTexture().getSize().x < b->getTexture().getSize().x; })
                          )->getTexture().getSize().x;

    const float padding = 2;
    items[0]->getSprite().setPosition(padding + (maxSize - items[0]->getTexture().getSize().x)/2,
                                      padding);
    for (size_t i = 1; i < items.size(); i++)
        items[i]->getSprite().setPosition(padding + (maxSize - items[i]->getTexture().getSize().x)/2,
                                          items[i - 1]->getSprite().getPosition().y + items[i - 1]->getTexture().getSize().y + padding);

    int ticks = 0;
    const int animationPeriod = 200;
    for (auto item: items)
    {
        float spriteSize = max(item->getTexture().getSize().x,
                               item->getTexture().getSize().y);

        const auto enchanted = dynamic_cast<EnchantedSprite*>(item);
        if (enchanted != nullptr)
        {
            enchanted->setUniform("texture", sf::Shader::CurrentTexture);
            enchanted->setUniform("size", spriteSize);
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;

                default: break;
            }
        }

        window.clear(sf::Color(139, 139, 139));
        for (auto& item: items)
            item->draw(window);
        window.display();

        ticks = (ticks + 1) % animationPeriod;
        for (auto item: items)
        {
            const auto enchanted = dynamic_cast<EnchantedSprite*>(item);
            if (enchanted != nullptr)
                enchanted->setUniform("ticks", (float)ticks / animationPeriod);
        }
    }
}
